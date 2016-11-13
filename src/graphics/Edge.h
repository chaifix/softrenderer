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
         m_x = e.m_x; 
         m_xStep = e.m_xStep; 
         m_yStart = e.m_yStart; 
         m_yEnd = e.m_yEnd;
     }
     float GetX() const { return m_x; }
     int GetYStart() const { return m_yStart; }
     int GetYEnd() const { return m_yEnd; }

     Edge(const Gradients& gradients, const Vertex2& minYVert, const Vertex2& maxYVert, int minYVertIndex);

     void Step()
     {
        m_x += m_xStep;
        color += colorStep;
     }

     Vector4 GetColor() const
     {
         return color; 
     }

private: 
    float m_x;
    float m_xStep;
    int m_yStart;
    int m_yEnd;
    Vector4 color; 
    Vector4 colorStep; 
};


#endif