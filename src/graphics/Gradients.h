#ifndef __GRADIENTS_H
#define __GRADIENTS_H
#include "../math/Vector4.h"

class Vertex2;

class Gradients
{
public:
    Vector4 GetColor(int loc) const { return color[loc]; }
    Vector4 GetColorXStep() const { return colorXStep; }
    Vector4 GetColorYStep() const { return colorYStep; }
    Gradients(const Vertex2& minYVert, const Vertex2& midYVert, const Vertex2&maxYVert);
private:
    Vector4 color[3];
    Vector4 colorXStep; 
    Vector4 colorYStep;
};

#endif