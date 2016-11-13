#include "Edge.h"
#include "../math/Vertex2.h"

Edge::Edge(const Gradients& gradients,const Vertex2& minYVert, const Vertex2& maxYVert, int minYVertIndex)
{
    m_yStart = (int)ceil(minYVert.GetY());
    m_yEnd = (int)ceil(maxYVert.GetY());

    float yDist = maxYVert.GetY() - minYVert.GetY();
    float xDist = maxYVert.GetX() - minYVert.GetX();

    float yPrestep = m_yStart - minYVert.GetY();
    m_xStep = (float)xDist / (float)yDist;
    m_x = minYVert.GetX() + yPrestep * m_xStep;

    float xPrestep = m_x - minYVert.GetX();

    color = gradients.GetColor(minYVertIndex).Add(
        gradients.GetColorYStep().Mul(yPrestep)).Add(
            gradients.GetColorXStep().Mul(xPrestep));
    colorStep = gradients.GetColorYStep().Add(gradients.GetColorXStep().Mul(m_xStep));
}