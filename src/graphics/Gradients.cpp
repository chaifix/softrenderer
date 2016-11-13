#include "Gradients.h"
#include "../math/Vector4.h"
#include "../math/Vertex2.h"

Gradients::Gradients(const Vertex2& minYVert, const Vertex2& midYVert, const Vertex2&maxYVert)
{
    color[0] = minYVert.GetColor(); 
    color[1] = midYVert.GetColor(); 
    color[2] = maxYVert.GetColor();

    float oneOverdX = 1.0f /
        (((midYVert.GetX() - maxYVert.GetX()) *
        (minYVert.GetY() - maxYVert.GetY())) -
            ((minYVert.GetX() - maxYVert.GetX()) *
            (midYVert.GetY() - maxYVert.GetY())));

    float oneOverdY = -oneOverdX;

    colorXStep =
        (((color[1].Sub(color[2])).Mul(
        (minYVert.GetY() - maxYVert.GetY()))).Sub(
            ((color[0].Sub(color[2])).Mul(
            (midYVert.GetY() - maxYVert.GetY()))))).Mul(oneOverdX);

    colorYStep =
        (((color[1].Sub(color[2])).Mul(
        (minYVert.GetX() - maxYVert.GetX()))).Sub(
            ((color[0].Sub(color[2])).Mul(
            (midYVert.GetX() - maxYVert.GetX()))))).Mul(oneOverdY);
}
