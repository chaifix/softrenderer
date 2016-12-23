#ifndef  __RENDERCONTEXT_H
#define  __RENDERCONTEXT_H
#include "Bitmap.h"
#include "../math/Vertex2.h"
#include "Gradients.h"

class Edge;

class RenderContext : public Bitmap
{
public:
    RenderContext();
    RenderContext(int width, int height);
    ~RenderContext();
    void FillTriangle(const Vertex2& v1, const Vertex2& v2, const Vertex2& v3, const Bitmap& texture);
    void ScanTriangle(const Vertex2& minYVert, const Vertex2 & midYVert,
        const Vertex2& maxYVert, bool handedness, const Bitmap& texture);
private:
    void ScanEdges(const Gradients& gradients,Edge & a, Edge & b, bool handedness, const Bitmap& texture);
    void DrawScanLine(const Gradients& gradients, const Edge & left, const Edge & right, int j, const Bitmap& texture);
private:
};

#endif 
