#include "Gradients.h"
#include "../math/Vector4.h"
#include "../math/Vertex2.h"

float Gradients::CalcXStep(float values[], Vertex minYVert, Vertex midYVert,
    Vertex maxYVert, float oneOverdX)
{
    return
        (((values[1] - values[2]) *
        (minYVert.GetY() - maxYVert.GetY())) -
            ((values[0] - values[2]) *
            (midYVert.GetY() - maxYVert.GetY()))) * oneOverdX;
}
float Gradients::CalcYStep(float values[], Vertex minYVert, Vertex midYVert,
    Vertex maxYVert, float oneOverdY)
{
    return
        (((values[1] - values[2]) *
        (minYVert.GetX() - maxYVert.GetX())) -
            ((values[0] - values[2]) *
            (midYVert.GetX() - maxYVert.GetX()))) * oneOverdY;
}

Gradients::Gradients(const Vertex& minYVert, const Vertex& midYVert, const Vertex&maxYVert)
{
    float oneOverdX = 1.0f /
        (((midYVert.GetX() - maxYVert.GetX()) *
        (minYVert.GetY() - maxYVert.GetY())) -
            ((minYVert.GetX() - maxYVert.GetX()) *
            (midYVert.GetY() - maxYVert.GetY())));

    float oneOverdY = -oneOverdX;

    // Note that the W component is the perspective Z value;
    // The Z component is the occlusion Z value
    oneOverZ[0] = 1.0f / minYVert.pos.GetW();
    oneOverZ[1] = 1.0f / midYVert.pos.GetW();
    oneOverZ[2] = 1.0f / maxYVert.pos.GetW();

    texCoordX[0] = minYVert.GetTexCoords().GetX() * oneOverZ[0];
    texCoordX[1] = midYVert.GetTexCoords().GetX() * oneOverZ[1];
    texCoordX[2] = maxYVert.GetTexCoords().GetX() * oneOverZ[2];

    texCoordY[0] = minYVert.GetTexCoords().GetY() * oneOverZ[0];
    texCoordY[1] = midYVert.GetTexCoords().GetY() * oneOverZ[1];
    texCoordY[2] = maxYVert.GetTexCoords().GetY() * oneOverZ[2];

    texCoordXXStep = CalcXStep(texCoordX, minYVert, midYVert, maxYVert, oneOverdX);
    texCoordXYStep = CalcYStep(texCoordX, minYVert, midYVert, maxYVert, oneOverdY);
    texCoordYXStep = CalcXStep(texCoordY, minYVert, midYVert, maxYVert, oneOverdX);
    texCoordYYStep = CalcYStep(texCoordY, minYVert, midYVert, maxYVert, oneOverdY);
    oneOverZXStep = CalcXStep(oneOverZ, minYVert, midYVert, maxYVert, oneOverdX);
    oneOverZYStep = CalcYStep(oneOverZ, minYVert, midYVert, maxYVert, oneOverdY);
}
