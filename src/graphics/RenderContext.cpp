#include "RenderContext.h"
#include "../math/Matrix4.h"
#include "Edge.h"

RenderContext::RenderContext(int width, int height)
    :Bitmap(width, height)
{
}

RenderContext::~RenderContext()
{
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

    float texCoordX = left.GetTexCoordX() + texCoordXXStep * xPrestep;
    float texCoordY = left.GetTexCoordY() + texCoordYXStep * xPrestep;
    float oneOverZ = left.GetOneOverZ() + oneOverZXStep * xPrestep;

    for (int i = xMin; i < xMax; i++)
    {
        float z = 1.0f / oneOverZ;
        int srcX = (int)((texCoordX * z) * (float)(texture.GetWidth() - 1) + 0.5f);
        int srcY = (int)((texCoordY * z) * (float)(texture.GetHeight() - 1) + 0.5f);

        CopyPixel(i, j, srcX, srcY, texture);
        oneOverZ += oneOverZXStep;
        texCoordX += texCoordXXStep;
        texCoordY += texCoordYXStep;
    }
}
typedef Vertex Vertex; 
typedef Matrix4 Matrix4f;

void RenderContext::FillTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Bitmap& texture)
{
    Matrix4f screenSpaceTransform =
         Matrix4f().InitScreenSpaceTransform(GetWidth() / 2.f, GetHeight() / 2.f);
    Vertex* minYVert = &(v1.Transform(screenSpaceTransform).PerspectiveDivide());
    Vertex* midYVert = &(v2.Transform(screenSpaceTransform).PerspectiveDivide());
    Vertex* maxYVert = &(v3.Transform(screenSpaceTransform).PerspectiveDivide());
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

