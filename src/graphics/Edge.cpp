#include "Edge.h"
#include "../math/Vertex2.h"

Edge::Edge(const Gradients& gradients,const Vertex& minYVert, const Vertex& maxYVert, int minYVertIndex)
{
    yStart = (int)ceil(minYVert.GetY());
    yEnd = (int)ceil(maxYVert.GetY());

    float yDist = maxYVert.GetY() - minYVert.GetY();
    float xDist = maxYVert.GetX() - minYVert.GetX();

    float yPrestep = yStart - minYVert.GetY();
    xStep = (float)xDist / (float)yDist;
    x = minYVert.GetX() + yPrestep * xStep;
    float xPrestep = x - minYVert.GetX(); 

    //float xPrestep = x - minYVert.GetX();

    //color = gradients.GetColor(minYVertIndex).Add(
    //    gradients.GetColorYStep().Mul(yPrestep)).Add(
    //        gradients.GetColorXStep().Mul(xPrestep));
    //colorStep = gradients.GetColorYStep().Add(gradients.GetColorXStep().Mul(xStep));

    texCoordX = gradients.GetTexCoordX(minYVertIndex) +
        gradients.GetTexCoordXXStep() * xPrestep +
        gradients.GetTexCoordXYStep() * yPrestep;
    texCoordXStep = gradients.GetTexCoordXYStep() + gradients.GetTexCoordXXStep() * xStep;

    texCoordY = gradients.GetTexCoordY(minYVertIndex) +
        gradients.GetTexCoordYXStep() * xPrestep +
        gradients.GetTexCoordYYStep() * yPrestep;
    texCoordYStep = gradients.GetTexCoordYYStep() + gradients.GetTexCoordYXStep() * xStep;

    oneOverZ = gradients.GetOneOverZ(minYVertIndex) +
        gradients.GetOneOverZXStep() * xPrestep +
        gradients.GetOneOverZYStep() * yPrestep;
    oneOverZStep = gradients.GetOneOverZYStep() + gradients.GetOneOverZXStep() * xStep;

}