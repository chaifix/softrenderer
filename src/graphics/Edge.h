#ifndef __EDGE_H
#define __EDGE_H

#include "Gradients.h"

class Vertex2;
class Vector4;

class Edge
{
public:
     Edge() {};
     Edge(const Edge&e)
     {
         x = e.x; 
         xStep = e.xStep; 
         yStart = e.yStart; 
         yEnd = e.yEnd;
     }
     float GetX() const { return x; }
     int GetYStart() const { return yStart; }
     int GetYEnd() const { return yEnd; }

     Edge(const Gradients& gradients, const Vertex2& minYVert, const Vertex2& maxYVert, int minYVertIndex);

     void Step()
     {
        x += xStep;
        //color += colorStep;
        texCoordX += texCoordXStep;
        texCoordY += texCoordYStep;
     }

     //Vector4 GetColor() const
     //{
     //    return color; 
     //}

     float GetTexCoordX() const
     {
         return texCoordX;
     }

     float GetTexCoordY() const
     {
         return texCoordY;
     }

private: 
    float x;
    float xStep;
    int yStart;
    int yEnd;
    //Vector4 color; 
    //Vector4 colorStep; 

    float texCoordX;
    float texCoordXStep;
    float texCoordY;
    float texCoordYStep;
};


#endif