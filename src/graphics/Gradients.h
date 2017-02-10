#ifndef __GRADIENTS_H
#define __GRADIENTS_H
#include "../math/Vector4.h"

class Vertex;

class Gradients
{
public:

    Gradients(const Vertex& minYVert, const Vertex& midYVert, const Vertex&maxYVert);

    float GetTexCoordX(int loc) const { return texCoordX[loc]; }
    float GetTexCoordY(int loc) const { return texCoordY[loc]; }

    float GetTexCoordXXStep() const { return texCoordXXStep; }
    float GetTexCoordXYStep() const { return texCoordXYStep; }
    float GetTexCoordYXStep() const { return texCoordYXStep; }
    float GetTexCoordYYStep() const { return texCoordYYStep; }
    float GetOneOverZXStep() const { return oneOverZXStep; }
    float GetOneOverZYStep() const { return oneOverZYStep; }
    float GetDepth(int loc)  const { return depth[loc]; }
    float GetOneOverZ(int loc) const { return oneOverZ[loc]; }

    float CalcXStep(float values[], Vertex minYVert, Vertex midYVert,
        Vertex maxYVert, float oneOverdX);
    float CalcYStep(float values[], Vertex minYVert, Vertex midYVert,
        Vertex maxYVert, float oneOverdY);

    float GetDepthXStep() const { return depthXStep; }
    float GetDepthYStep() const { return depthYStep; }

private:

    float texCoordX[3];
    float texCoordY[3];
    float oneOverZ[3];
    float depth[3];

    float texCoordXXStep;
    float texCoordXYStep;
    float texCoordYXStep;
    float texCoordYYStep;
    
    float oneOverZXStep;
    float oneOverZYStep;

    float depthXStep;
    float depthYStep;
};

#endif