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
    Edge topToBottom = Edge(minYVert, maxYVert);
    Edge topToMiddle = Edge(minYVert, midYVert);
    Edge middleToBottom = Edge(midYVert, maxYVert);

    ScanEdges(topToBottom, topToMiddle, handedness);
    ScanEdges(topToBottom, middleToBottom, handedness);
}

void RenderContext::ScanEdges(Edge& a,Edge& b, bool handedness)
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
        DrawScanLine(*left, *right, j);
        (*left).Step();
        (*right).Step();
    }
}

void RenderContext::DrawScanLine(const Edge& left,const Edge& right, int j)
{
    int xMin = (int)ceil(left.GetX());
    int xMax = (int)ceil(right.GetX());

    for (int i = xMin; i < xMax; i++)
    {
        DrawPixel(i, j, 0xFF, 0xFF, 0xFF, 0xFF);
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

