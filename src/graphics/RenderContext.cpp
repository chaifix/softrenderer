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
   const Vertex2& maxYVert, bool handedness)
{
    Gradients gradients = Gradients(minYVert, midYVert, maxYVert);
    Edge topToBottom = Edge(gradients,minYVert, maxYVert, 0);
    Edge topToMiddle = Edge(gradients,minYVert, midYVert,  0);
    Edge middleToBottom = Edge(gradients, midYVert, maxYVert, 1);

    ScanEdges(gradients, topToBottom, topToMiddle, handedness);
    ScanEdges(gradients, topToBottom, middleToBottom, handedness);
}

void RenderContext::ScanEdges(const Gradients& gradients, Edge& a,Edge& b, bool handedness)
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
        DrawScanLine(gradients, *left, *right, j);
        (*left).Step();
        (*right).Step();
    }
}

void RenderContext::DrawScanLine(const Gradients& gradients, const Edge& left,const Edge& right, int j)
{
    int xMin = (int)ceil(left.GetX());
    int xMax = (int)ceil(right.GetX());
    float xPrestep = xMin - left.GetX(); 

    Vector4 color = left.GetColor().Add(gradients.GetColorXStep().Mul(xPrestep));

    for (int i = xMin; i < xMax; i++)
    {
        uchar r = color.x * 255.f + 0.5f; 
        uchar g = color.y * 255.f + 0.5f;
        uchar b = color.z * 255.f + 0.5f;

        DrawPixel(i, j, r, g, b, 0xFF);
        color = color.Add(gradients.GetColorXStep());
    }
}
typedef Vertex2 Vertex; 
typedef Matrix4 Matrix4f;

void RenderContext::FillTriangle(const Vertex2& v1, const Vertex2& v2, const Vertex2& v3)
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
        (*minYVert).TriangleAreaTimesTwo(*maxYVert, *midYVert) >= 0);
}

