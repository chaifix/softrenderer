#ifndef __QUATERNION_H
#define __QUATERNION_H
#include <cmath>
#include "Vector4.h"
#include "Matrix4.h"

// 四元数
class Quaternion
{
public:

public:
    float x, y, z, w; 
    Quaternion(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w)
    {
    }
    Quaternion(const Vector4& axis, float angle)
    {
        using std::sin; 
        using std::cos; 

        float sinHalfAngle = (float)sin(angle / 2);
        float cosHalfAngle = (float)cos(angle / 2);

        x = axis.x * sinHalfAngle;
        y = axis.y * sinHalfAngle;
        z = axis.z * sinHalfAngle;
        w = cosHalfAngle;
    }
    Quaternion(const Quaternion& q)
        :x(q.x), y(q.y), z(q.z), w(q.w)
    {
    }
    Quaternion(Quaternion&& q)
        :x(q.x), y(q.y), z(q.z), w(q.w)
    {
    }
    float Length() const
    {
        using std::sqrt;
        return (float)sqrt(x * x + y * y + z * z + w * w);
    }
    Quaternion Normal() const
    {
        float length = Length(); 
        return Quaternion(x / length, y / length, z / length, w / length);
    }
    // 共轭
    Quaternion Conjugate() const 
    {
        return Quaternion(-x, -y, -z, w);
    }
    Quaternion Mul(float r) const
    {
        return Quaternion(x * r, y * r, z * r, w * r);
    }
    Quaternion Mul(const Quaternion& r) const
    {
        float w_ = w * r.w - x * r.x - y * r.y - z * r.z;
        float x_ = x * r.w + w * r.x + y * r.z - z * r.y;
        float y_ = y * r.w + w * r.y + z * r.x - x * r.z;
        float z_ = z * r.w + w * r.z + x * r.y - y * r.x;

        return Quaternion(x_, y_, z_, w_);
    }
    Quaternion Mul(const Vector4& r) const
    {
        float w_ = -x * r.x - y * r.y - z * r.z;
        float x_ = w * r.x + y * r.z - z * r.y;
        float y_ = w * r.y + z * r.x - x * r.z;
        float z_ = w * r.z + x * r.y - y * r.x;

        return  Quaternion(x_, y_, z_, w_);
    }
    Quaternion Sub(Quaternion r) const
    {
        return  Quaternion(x - r.x, y - r.y, z - r.z, w - r.w);
    }
    Quaternion Add(Quaternion r) const
    {
        return  Quaternion(x + r.x, y + r.y, z + r.z, w + r.w);
    }
    Matrix4 ToRotationMatrix() const
    {
        Vector4 forward =  Vector4(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
        Vector4 up =  Vector4(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
        Vector4 right =  Vector4(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

        return  Matrix4().InitRotation(forward, up, right);
    }
    float Dot(Quaternion r)
    {
        return x * r.x + y * r.y + z * r.z + w * r.w;
    }
    Quaternion NLerp(Quaternion dest, float lerpFactor, bool shortest)
    {
        Quaternion correctedDest = dest;

        if (shortest && Dot(dest) < 0)
            correctedDest =  Quaternion(-dest.x, -dest.y, -dest.z, -dest.w);

        return correctedDest.Sub(*this).Mul(lerpFactor).Add(*this).Normal();
    }
    Quaternion SLerp(Quaternion dest, float lerpFactor, bool shortest)
    {
        float EPSILON = 1e3f;

        float cos = Dot(dest);
        Quaternion correctedDest = dest;

        if (shortest && cos < 0)
        {
            cos = -cos;
            correctedDest = Quaternion(-dest.x, -dest.y, -dest.z, -dest.w);
        }

        if (std::abs(cos) >= 1 - EPSILON)
            return NLerp(correctedDest, lerpFactor, false);

        float sin = (float)std::sqrt(1.0f - cos * cos);
        float angle = (float)std::atan2(sin, cos);
        float invSin = 1.0f / sin;

        float srcFactor = (float)std::sin((1.0f - lerpFactor) * angle) * invSin;
        float destFactor = (float)std::sin((lerpFactor)* angle) * invSin;

        return Mul(srcFactor).Add(correctedDest.Mul(destFactor));
    }
    Quaternion(const Matrix4& rot)
    {
        float trace = rot.Get(0, 0) + rot.Get(1, 1) + rot.Get(2, 2);

        if (trace > 0)
        {
            float s = 0.5f / (float)std::sqrt(trace + 1.0f);
            w = 0.25f / s;
            x = (rot.Get(1, 2) - rot.Get(2, 1)) * s;
            y = (rot.Get(2, 0) - rot.Get(0, 2)) * s;
            z = (rot.Get(0, 1) - rot.Get(1, 0)) * s;
        }
        else
        {
            if (rot.Get(0, 0) > rot.Get(1, 1) && rot.Get(0, 0) > rot.Get(2, 2))
            {
                float s = 2.0f * (float)std::sqrt(1.0f + rot.Get(0, 0) - rot.Get(1, 1) - rot.Get(2, 2));
                w = (rot.Get(1, 2) - rot.Get(2, 1)) / s;
                x = 0.25f * s;
                y = (rot.Get(1, 0) + rot.Get(0, 1)) / s;
                z = (rot.Get(2, 0) + rot.Get(0, 2)) / s;
            }
            else if (rot.Get(1, 1) > rot.Get(2, 2))
            {
                float s = 2.0f * (float)std::sqrt(1.0f + rot.Get(1, 1) - rot.Get(0, 0) - rot.Get(2, 2));
                w = (rot.Get(2, 0) - rot.Get(0, 2)) / s;
                x = (rot.Get(1, 0) + rot.Get(0, 1)) / s;
                y = 0.25f * s;
                z = (rot.Get(2, 1) + rot.Get(1, 2)) / s;
            }
            else
            {
                float s = 2.0f * (float)std::sqrt(1.0f + rot.Get(2, 2) - rot.Get(0, 0) - rot.Get(1, 1));
                w = (rot.Get(0, 1) - rot.Get(1, 0)) / s;
                x = (rot.Get(2, 0) + rot.Get(0, 2)) / s;
                y = (rot.Get(1, 2) + rot.Get(2, 1)) / s;
                z = 0.25f * s;
            }
        }

        float length = (float)std::sqrt(x * x + y * y + z * z + w * w);
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    Quaternion& operator = (const Quaternion& q)
    {
        x = q.x; 
        y = q.y; 
        z = q.z;
        w = q.w;
        return *this;
    }

    Vector4 GetForward()
    {
        return  Vector4(0, 0, 1, 1).Rotate(*this);
    }

    Vector4 GetBack()
    {
        return  Vector4(0, 0, -1, 1).Rotate(*this);
    }

    Vector4 GetUp()
    {
        return  Vector4(0, 1, 0, 1).Rotate(*this);
    }

    Vector4 GetDown()
    {
        return  Vector4(0, -1, 0, 1).Rotate(*this);
    }

    Vector4 GetRight()
    {
        return  Vector4(1, 0, 0, 1).Rotate(*this);
    }

    Vector4 GetLeft()
    {
        return  Vector4(-1, 0, 0, 1).Rotate(*this);
    }

private:
};

#endif