#ifndef __EDGE_H
#define __EDGE_H

#include "Gradients.h"

class Vertex;
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

     Edge(const Gradients& gradients, const Vertex& minYVert, const Vertex& maxYVert, int minYVertIndex);

     void Step()
     {
        x += xStep;
        //color += colorStep;
        texCoordX += texCoordXStep;
        texCoordY += texCoordYStep;
        oneOverZ += oneOverZStep;
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

     float GetOneOverZ() const { return oneOverZ; }

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
    float oneOverZ; 
    float oneOverZStep; 
};


#endif