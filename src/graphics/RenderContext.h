#ifndef  __RENDERCONTEXT_H
#define  __RENDERCONTEXT_H
#include "Bitmap.h"
#include "../math/Vertex2.h"
#include "Gradients.h"
#include "Mesh.h"

class Edge;

class RenderContext : public Bitmap
{
public:
    RenderContext();
    RenderContext(int width, int height);
    ~RenderContext();
    void DrawMesh(const Mesh& mesh, const Matrix4f& viewProjection, const Matrix4& transform, const Bitmap& texture);
    void ScanTriangle(const Vertex& minYVert, const Vertex & midYVert,
        const Vertex& maxYVert, bool handedness, const Bitmap& texture);
    void ClearDepthBuffer();
private:
    void DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Bitmap& texture);
    void FillTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Bitmap& texture);
    // ²Ã¼ô
    bool ClipPolygonAxis(std::vector<Vertex>& vertices, std::vector<Vertex>& auxillaryList, int componentIndex);
    void ClipPolygonComponent(std::vector<Vertex>& vertices, int componentIndex, float componentFactor, std::vector<Vertex>& result);
    void ScanEdges(const Gradients& gradients,Edge & a, Edge & b, bool handedness, const Bitmap& texture);
    void DrawScanLine(const Gradients& gradients, const Edge & left, const Edge & right, int j, const Bitmap& texture);
private:
    float* zbuffer;
    int w, h;
};

#endif 
