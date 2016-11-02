#ifndef __VERTEX_H
#define __VERTEX_H

#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4.h"

class Vertex
{
public:
    Vertex(float x, float y)
    {
        this->pos.x = x; 
        this->pos.y = y; 
    }
    Vertex(const Vector4& _pos, const Vector4& _texCoord, const Vector4 _normal)
        : pos(_pos), texCoords(_texCoord), normal(_normal)
    {
    }

    Vertex Transform(const Matrix4& transform, const Matrix4 &normalTransform)
    {
        return Vertex(transform.Transform(pos), texCoords,
            normalTransform.Transform(normal).Normal());
    }
    Vertex PerspectiveDivide()
    {
        return Vertex(Vector4(pos.x / pos.w, pos.y / pos.w,
            pos.z / pos.w, pos.w),
            texCoords, normal);
    }

    float TriangleAreaTimesTwo(const Vertex& b, Vertex c)
    {
        float x1 = b.pos.x - pos.x;
        float y1 = b.pos.y - pos.y;

        float x2 = c.pos.x - pos.x;
        float y2 = c.pos.y - pos.y;

        return (x1 * y2 - x2 * y1);
    }

    Vertex Lerp(Vertex other, float lerpAmt)
    {
        return  Vertex(
            pos.Lerp(other.pos, lerpAmt),
            texCoords.Lerp(other.texCoords, lerpAmt),
            normal.Lerp(other.normal, lerpAmt)
        );
    }

    bool IsInsideViewFrustum()
    {
        return
            std::abs(pos.x) <= std::abs(pos.w) &&
            std::abs(pos.y) <= std::abs(pos.w) &&
            std::abs(pos.z) <= std::abs(pos.w);
    }

public:
    Vector4 pos; // ×ø±ê
    Vector4 texCoords; // UV
    Vector4 normal; // ·¨Ïß

};

#endif