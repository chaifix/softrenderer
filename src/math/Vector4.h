#ifndef __VECTOR4_H
#define __VECTOR4_H
#include <cmath>
#include <string>
#include <sstream>

class Quaternion;

class Vector4
{
public:
    Vector4() {}
    Vector4(float _x, float _y, float _z, float _w = 1.f)
        :x(_x), y(_y), z(_z), w(_w)
    {
    }
    float Length() const 
    {
        return std::sqrt(x* x + y*y + z*z + w*w); 
    }
    float Length2() const
    {
        return x*x + y*y + z*z + w*w; 
    }
    float Max()
    {
        using std::fmaxf; 
        return fmaxf(fmaxf(x, y), fmaxf(z, w));
    }
    float Dot(const Vector4& r) const 
    {
        return x * r.x + y * r.y + z * r.z + w * r.w; 
    }
    Vector4 Cross(const Vector4& r) const 
    {
        float _x = y * r.z - z * r.y; 
        float _y = z * r.x - x * r.z; 
        float _z = x * r.y - y * r.x;
        return Vector4(_x, _y, _z, 0); 
    }
    Vector4 Normal() const 
    {
        float length = Length(); 
        return Vector4(x / length, y / length, z / length, w / length); 
    }
    void Normalize()
    {
        float length = Length(); 
        x /= length; 
        y /= length; 
        z /= length; 
        w /= length;
    }
    // 绕任意轴旋转
    Vector4 Rotate(const Vector4& axis, float angle)
    {
        float sinAngle = std::sin(-angle); 
        float cosAngle = std::cos(-angle); 
        return Cross(axis.Mul(sinAngle)).Add(            //Rotation on local X
            (Mul(cosAngle)).Add(                         //Rotation on local Z
                axis.Mul(Dot(axis.Mul(1 - cosAngle))))); //Rotation on local Y
    }
    // 插值
    Vector4 Lerp(const Vector4& dst, float lerpFactor)
    {
        return dst.Sub(*this).Mul(lerpFactor).Add(*this);
    }
    Vector4 Rotate(const Quaternion& rotation);
    Vector4 Add(const Vector4& r) const
    {
        return Vector4(x + r.x, y + r.y, z + r.z, w + r.w);
    }
    Vector4 Add(float r) const
    {
        return Vector4(x + r, y + r, z + r, w + r);
    }
    Vector4 Sub(const Vector4& r) const
    {
        return Vector4(x - r.x, y - r.y, z - r.z, w - r.w);
    }
    Vector4 Sub(float r) const
    {
        return Vector4(x - r, y - r, z - r, w - r);
    }
    Vector4 Mul(const Vector4& r) const
    {
        return Vector4(x * r.x, y * r.y, z * r.z, w * r.w);
    }
    Vector4 Mul(float r) const
    {
        return Vector4(x * r, y * r, z * r, w * r);
    }
    Vector4 Div(const Vector4& r) const
    {
        return Vector4(x / r.x, y / r.y, z / r.z, w / r.w);
    }
    Vector4 Div(float r) const 
    {
        return Vector4(x / r, y / r, z / r, w / r);
    }
    Vector4 Abs() const
    {
        using std::abs;
        return Vector4(abs(x), abs(y), abs(z), abs(w));
    }
    Vector4 operator + (const Vector4& r) const
    {
        return Vector4(x + r.x, y + r.y, z + r.z, w + r.w); 
    }
    Vector4 operator + (float n) const
    {
        return Vector4(x + n, y + n, z + n, w + n); 
    }
    Vector4& operator += (const Vector4& r)
    {
        x += r.x; 
        y += r.y; 
        z += r.z;
        w += r.w;
        return *this;
    }
    Vector4 operator - (const Vector4 r) const
    {
        return Vector4(x - r.x, y - r.y, z - r.z, w - r.w);
    }
    Vector4 operator - (float n) const
    {
        return Vector4(x - n, y - n, z - n, w - n);
    }
    bool operator == (const Vector4& r)const
    {
        return x == r.x && y == r.y && z == r.z && w == r.w; 
    }
    // 点乘
    float operator * (const Vector4 r) const
    {
        return x * r.x + y * r.y + z * r.z + w * r.w; 
    }
    // 数乘
    Vector4 operator * (float n) const
    {
        return Vector4(x * n, y * n, z * n, w * n);
    }
    friend Vector4 operator * (float l, const Vector4& r)
    {
        return Vector4(l*r.x, l*r.y, l*r.z, l*r.w);
    }
    // 数除
    Vector4 operator / (float n) const
    {
        return Vector4(x / n, y / n, z / n, w / n);
    }
    std::string ToString()
    {
        using std::string; 
        using std::stringstream;
        string s; 
        stringstream ss(s);
        ss << "(" << x << "," << y << "," << z << ")";
        return s;
    }

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetZ() const { return z; }
    float GetW() const { return w; }

public:
    float x, y, z, w; 
};

Vector4 operator * (float l, const Vector4& r);

typedef Vector4 Vector4f;

#endif