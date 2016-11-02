#include "Edge.h"
#include "../math/Vertex2.h"

Edge::Edge(const Vertex2& minYVert, const Vertex2& maxYVert)
{
    m_yStart = (int)ceil(minYVert.GetY());
    m_yEnd = (int)ceil(maxYVert.GetY());

    float yDist = maxYVert.GetY() - minYVert.GetY();
    float xDist = maxYVert.GetX() - minYVert.GetX();

    float yPrestep = m_yStart - minYVert.GetY();
    m_xStep = (float)xDist / (float)yDist;
    m_x = minYVert.GetX() + yPrestep * m_xStep;
}