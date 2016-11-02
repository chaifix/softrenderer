#ifndef __EDGE_H
#define __EDGE_H

class Vertex2;

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

     Edge(const Vertex2& minYVert, const Vertex2& maxYVert);

     void Step()
     {
        m_x += m_xStep;
     }

private: 
    float m_x;
    float m_xStep;
    int m_yStart;
    int m_yEnd;
};


#endif