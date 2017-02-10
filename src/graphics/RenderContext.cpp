#include "RenderContext.h"
#include "../math/Matrix4.h"
#include "Edge.h"
#include <cfloat>

RenderContext::RenderContext(int width, int height)
    :Bitmap(width, height), w(width), h(height)
{
    zbuffer = new float[width * height];
}

RenderContext::~RenderContext()
{
    delete[] zbuffer;
}

void RenderContext::ClearDepthBuffer()
{
    for (int i = 0; i < w*h; i++)
    {
        zbuffer[i] = DBL_MAX;
    }
}

void RenderContext::ScanTriangle(const Vertex& minYVert, const Vertex& midYVert,
   const Vertex& maxYVert, bool handedness, const Bitmap& texture)
{
    Gradients gradients = Gradients(minYVert, midYVert, maxYVert);
    Edge topToBottom = Edge(gradients,minYVert, maxYVert, 0);
    Edge topToMiddle = Edge(gradients,minYVert, midYVert,  0);
    Edge middleToBottom = Edge(gradients, midYVert, maxYVert, 1);

    ScanEdges(gradients, topToBottom, topToMiddle, handedness, texture);
    ScanEdges(gradients, topToBottom, middleToBottom, handedness, texture);
}

void RenderContext::ScanEdges(const Gradients& gradients, Edge& a,Edge& b, bool handedness, const Bitmap& texture)
{
    Edge* left = &a;
    Edge* right = &b;
    if (handedness)
    {
        Edge* temp = left;
        left = right;
        right = temp;
    }

    int yStart = b.GetYStart();
    int yEnd = b.GetYEnd();
    for (int j = yStart; j < yEnd; j++)
    {
        DrawScanLine(gradients, *left, *right, j, texture);
        (*left).Step();
        (*right).Step();
    }
}

void RenderContext::DrawScanLine(const Gradients& gradients, const Edge& left,const Edge& right, int j, const Bitmap& texture)
{
    int xMin = (int)ceil(left.GetX());
    int xMax = (int)ceil(right.GetX());
    float xPrestep = xMin - left.GetX(); 

    //Vector4 color = left.GetColor().Add(gradients.GetColorXStep().Mul(xPrestep));
    float xDist = right.GetX() - left.GetX();
    float texCoordXXStep = (right.GetTexCoordX() - left.GetTexCoordX()) / xDist;
    float texCoordYXStep = (right.GetTexCoordY() - left.GetTexCoordY()) / xDist;
    float oneOverZXStep = (right.GetOneOverZ() - left.GetOneOverZ()) / xDist;
    float depthXStep = (right.GetDepth() - left.GetDepth()) / xDist;

    float texCoordX = left.GetTexCoordX() + texCoordXXStep * xPrestep;
    float texCoordY = left.GetTexCoordY() + texCoordYXStep * xPrestep;
    float oneOverZ = left.GetOneOverZ() + oneOverZXStep * xPrestep;
    float depth = left.GetDepth() + depthXStep * xPrestep;

    for (int i = xMin; i < xMax; i++)
    {
        int index = i + j * GetWidth();
        if (depth < zbuffer[index])
        {
            zbuffer[index] = depth;
            float z = 1.0f / oneOverZ;
            int srcX = (int)((texCoordX * z) * (float)(texture.GetWidth() - 1) + 0.5f);
            int srcY = (int)((texCoordY * z) * (float)(texture.GetHeight() - 1) + 0.5f);

            CopyPixel(i, j, srcX, srcY, texture);
        }

        oneOverZ += oneOverZXStep;
        texCoordX += texCoordXXStep;
        texCoordY += texCoordYXStep;
        depth += depthXStep;
    }
}

typedef Vertex Vertex; 
typedef Matrix4 Matrix4f;

void RenderContext::FillTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Bitmap& texture)
{
    Matrix4f screenSpaceTransform =
         Matrix4f().InitScreenSpaceTransform(GetWidth() / 2.f, GetHeight() / 2.f);
    Matrix4f identity = Matrix4f().LoadIdentity();
    Vertex* minYVert = &(v1.Transform(screenSpaceTransform, identity).PerspectiveDivide());
    Vertex* midYVert = &(v2.Transform(screenSpaceTransform, identity).PerspectiveDivide());
    Vertex* maxYVert = &(v3.Transform(screenSpaceTransform, identity).PerspectiveDivide());

    // ±³ÃæÌÞ³ý
    if (minYVert->TriangleAreaTimesTwo(*maxYVert, *midYVert) >= 0)
    {
        return;
    }

    Vertex* temp;

    if (maxYVert->GetY() < midYVert->GetY())
    {
        temp = maxYVert;
        maxYVert = midYVert;
        midYVert = temp;
    }

    if (midYVert->GetY() < minYVert->GetY())
    {
        temp = midYVert;
        midYVert = minYVert;
        minYVert = temp;
    }

    if (maxYVert->GetY() < midYVert->GetY())
    {
        temp = maxYVert;
        maxYVert = midYVert;
        midYVert = temp;
    }

    ScanTriangle(*minYVert, 
        *midYVert,
        *maxYVert,
        (*minYVert).TriangleAreaTimesTwo(*maxYVert, *midYVert) >= 0, texture);
}

void RenderContext::DrawMesh(const Mesh& mesh, const Matrix4f& viewProjection, const Matrix4& transform, const Bitmap& texture)
{
    Matrix4f mvp = viewProjection.Mul(transform);
    for (int i = 0; i < mesh.indices.size(); i+=3)
    {
        DrawTriangle(
            mesh.vertices[mesh.indices[i]]->Transform(mvp, transform),
            mesh.vertices[mesh.indices[i + 1]]->Transform(mvp, transform),
            mesh.vertices[mesh.indices[i + 2]]->Transform(mvp, transform),
            texture
        );
    }
}

bool RenderContext::ClipPolygonAxis(std::vector<Vertex>& vertices, std::vector<Vertex>& auxillaryList, int componentIndex)
{
    ClipPolygonComponent(vertices, componentIndex, 1.f, auxillaryList);
    vertices.clear(); 
    if (auxillaryList.empty())
    {
        return false; 
    }
    ClipPolygonComponent(auxillaryList, componentIndex, -1.f, vertices);
    auxillaryList.clear(); 
    return !vertices.empty();
}

void RenderContext::ClipPolygonComponent(std::vector<Vertex>& vertices, int componentIndex, float componentFactor, std::vector<Vertex>& result)
{
    Vertex* pPreviousVertex = &vertices[vertices.size() - 1];
    float previousComponent = (*pPreviousVertex)[componentIndex] * componentFactor;
    bool previousInside = previousComponent <= pPreviousVertex->pos.GetW();
    std::vector<Vertex>::iterator it = vertices.begin();
    while (it != vertices.end())
    {
        Vertex& currentVertex = *it;
        float currentComponent = currentVertex[componentIndex] * componentFactor;
        bool currentInside = currentComponent <= currentVertex.pos.w; 

        if (currentInside ^ previousInside)
        {
            float lerpAmt = (pPreviousVertex->pos.GetW() - previousComponent) /
                ((pPreviousVertex->pos.GetW() - previousComponent) -
                (currentVertex.pos.GetW() - currentComponent));
            result.push_back(pPreviousVertex->Lerp(currentVertex, lerpAmt));
        }

        if (currentInside)
        {
            result.push_back(currentVertex);
        }

        pPreviousVertex = &currentVertex;
        previousComponent = currentComponent; 
        previousInside = currentInside;

        it++;
    }
}

void RenderContext::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Bitmap& texture)
{
    bool v1Inside = v1.IsInsideViewFrustum();
    bool v2Inside = v2.IsInsideViewFrustum();
    bool v3Inside = v3.IsInsideViewFrustum();

    if (v1Inside && v2Inside && v3Inside) {
        FillTriangle(v1, v2, v3, texture);
        return;
    }

    if (!v1Inside && !v2Inside && !v3Inside) {
        return;
    }
    using std::vector;
    vector<Vertex> vertices;
    vector<Vertex> auxillaryList;

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    if (ClipPolygonAxis(vertices, auxillaryList, 0) && ClipPolygonAxis(vertices, auxillaryList, 1)
        && ClipPolygonAxis(vertices, auxillaryList, 2)) {
        Vertex initialVertex = vertices[0];

        for (int i = 1; i < vertices.size() - 1; i++) {
            FillTriangle(initialVertex, vertices[i], vertices[i + 1], texture);
        }
    }

}
