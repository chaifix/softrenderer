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

void RenderContext::ScanTriangle(const Vertex2& minYVert, const Vertex2& midYVert,
   const Vertex2& maxYVert, bool handedness, const Bitmap& texture)
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
    float texCoordX = left.GetTexCoordX() + gradients.GetTexCoordXXStep() * xPrestep;
    float texCoordY = left.GetTexCoordY() + gradients.GetTexCoordYXStep() * xPrestep;

    for (int i = xMin; i < xMax; i++)
    {
        int srcX = (int)(texCoordX * (texture.GetWidth() - 1) + 0.5f);

        //Point out that this was changed to get height in video 16
        int srcY = (int)(texCoordY * (texture.GetHeight() - 1) + 0.5f);

        CopyPixel(i, j, srcX, srcY, texture);
        texCoordX += gradients.GetTexCoordXXStep();
        texCoordY += gradients.GetTexCoordYXStep();
    }
}
typedef Vertex2 Vertex; 
typedef Matrix4 Matrix4f;

void RenderContext::FillTriangle(const Vertex2& v1, const Vertex2& v2, const Vertex2& v3, const Bitmap& texture)
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

