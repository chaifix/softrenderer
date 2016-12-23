#include "Gradients.h"
#include "../math/Vector4.h"
#include "../math/Vertex2.h"

Gradients::Gradients(const Vertex2& minYVert, const Vertex2& midYVert, const Vertex2&maxYVert)
{
    float oneOverdX = 1.0f /
        (((midYVert.GetX() - maxYVert.GetX()) *
        (minYVert.GetY() - maxYVert.GetY())) -
            ((minYVert.GetX() - maxYVert.GetX()) *
            (midYVert.GetY() - maxYVert.GetY())));

    float oneOverdY = -oneOverdX;

    texCoordX[0] = minYVert.GetTexCoords().GetX();
    texCoordX[1] = midYVert.GetTexCoords().GetX();
    texCoordX[2] = maxYVert.GetTexCoords().GetX();

    texCoordY[0] = minYVert.GetTexCoords().GetY();
    texCoordY[1] = midYVert.GetTexCoords().GetY();
    texCoordY[2] = maxYVert.GetTexCoords().GetY();

    texCoordXXStep =
        (((texCoordX[1] - texCoordX[2]) *
        (minYVert.GetY() - maxYVert.GetY())) -
            ((texCoordX[0] - texCoordX[2]) *
            (midYVert.GetY() - maxYVert.GetY()))) * oneOverdX;

    texCoordXYStep =
        (((texCoordX[1] - texCoordX[2]) *
        (minYVert.GetX() - maxYVert.GetX())) -
            ((texCoordX[0] - texCoordX[2]) *
            (midYVert.GetX() - maxYVert.GetX()))) * oneOverdY;

    texCoordYXStep =
        (((texCoordY[1] - texCoordY[2]) *
        (minYVert.GetY() - maxYVert.GetY())) -
            ((texCoordY[0] - texCoordY[2]) *
            (midYVert.GetY() - maxYVert.GetY()))) * oneOverdX;

    texCoordYYStep =
        (((texCoordY[1] - texCoordY[2]) *
        (minYVert.GetX() - maxYVert.GetX())) -
            ((texCoordY[0] - texCoordY[2]) *
            (midYVert.GetX() - maxYVert.GetX()))) * oneOverdY;

}
