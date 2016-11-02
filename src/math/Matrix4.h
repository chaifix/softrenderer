#ifndef __MATRIX4_H
#define __MATRIX4_H
#include "Vector4.h"

class Matrix4
{
public:
    Matrix4() {}; 

    Matrix4(const Matrix4& mat)
    {
        memcpy(m, mat.m, sizeof(float) * 16);
    }

    Matrix4(Matrix4&& mat)
    {
        memcpy(m, mat.m, sizeof(float) * 16);
    }

    Matrix4& LoadIdentity()
    {
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this; 
    }

    Matrix4& InitScreenSpaceTransform(float halfWidth, float halfHeight) {
        m[0][0] = halfWidth;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = halfWidth - 0.5f;
        m[1][0] = 0;
        m[1][1] = -halfHeight;
        m[1][2] = 0;
        m[1][3] = halfHeight - 0.5f;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this;
    }

    Matrix4 InitTranslation(float x, float y, float z)
    {
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = x;
        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = y;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = z;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this;
    }
    
    Matrix4& InitRotation(float x, float y, float z, float angle) {
        float sin = (float)std::sin(angle);
        float cos = (float)std::cos(angle);

        m[0][0] = cos + x * x * (1 - cos);
        m[0][1] = x * y * (1 - cos) - z * sin;
        m[0][2] = x * z * (1 - cos) + y * sin;
        m[0][3] = 0;
        m[1][0] = y * x * (1 - cos) + z * sin;
        m[1][1] = cos + y * y * (1 - cos);
        m[1][2] = y * z * (1 - cos) - x * sin;
        m[1][3] = 0;
        m[2][0] = z * x * (1 - cos) - y * sin;
        m[2][1] = z * y * (1 - cos) + x * sin;
        m[2][2] = cos + z * z * (1 - cos);
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this;
    }

    // 旋转矩阵
    Matrix4& InitRotation(float x, float y, float z)
    {
        Matrix4 rx = Matrix4();
        Matrix4 ry = Matrix4();
        Matrix4 rz = Matrix4();

        rz.m[0][0] = (float)std::cos(z);
        rz.m[0][1] = -(float)std::sin(z);
        rz.m[0][2] = 0;
        rz.m[0][3] = 0;
        rz.m[1][0] = (float)std::sin(z);
        rz.m[1][1] = (float)std::cos(z);
        rz.m[1][2] = 0;
        rz.m[1][3] = 0;
        rz.m[2][0] = 0;
        rz.m[2][1] = 0;
        rz.m[2][2] = 1;
        rz.m[2][3] = 0;
        rz.m[3][0] = 0;
        rz.m[3][1] = 0;
        rz.m[3][2] = 0;
        rz.m[3][3] = 1;

        rx.m[0][0] = 1;
        rx.m[0][1] = 0;
        rx.m[0][2] = 0;
        rx.m[0][3] = 0;
        rx.m[1][0] = 0;
        rx.m[1][1] = (float)std::cos(x);
        rx.m[1][2] = -(float)std::sin(x);
        rx.m[1][3] = 0;
        rx.m[2][0] = 0;
        rx.m[2][1] = (float)std::sin(x);
        rx.m[2][2] = (float)std::cos(x);
        rx.m[2][3] = 0;
        rx.m[3][0] = 0;
        rx.m[3][1] = 0;
        rx.m[3][2] = 0;
        rx.m[3][3] = 1;

        ry.m[0][0] = (float)std::cos(y);
        ry.m[0][1] = 0;
        ry.m[0][2] = -(float)std::sin(y);
        ry.m[0][3] = 0;
        ry.m[1][0] = 0;
        ry.m[1][1] = 1;
        ry.m[1][2] = 0;
        ry.m[1][3] = 0;
        ry.m[2][0] = (float)std::sin(y);
        ry.m[2][1] = 0;
        ry.m[2][2] = (float)std::cos(y);
        ry.m[2][3] = 0;
        ry.m[3][0] = 0;
        ry.m[3][1] = 0;
        ry.m[3][2] = 0;
        ry.m[3][3] = 1;

        Matrix4 res = rz * (ry * rx);
        float** temp = (float**)res.m;

        memcpy(m, temp, sizeof(float)*16);

        return *this;
    }
    
    // 缩放矩阵
    Matrix4& InitScale(float x, float y, float z)
    {
        m[0][0] = x;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = y;
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = z;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this;
    }

    // 透视投影
    Matrix4& InitPerspective(float fov, float aspectRatio, float zNear, float zFar)
    {
        float tanHalfFOV = (float)std::tan(fov / 2);
        float zRange = zNear - zFar;

        m[0][0] = 1.0f / (tanHalfFOV * aspectRatio);
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;
        m[1][0] = 0;
        m[1][1] = 1.0f / tanHalfFOV;
        m[1][2] = 0;
        m[1][3] = 0;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = (-zNear - zFar) / zRange;
        m[2][3] = 2 * zFar * zNear / zRange;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 1;
        m[3][3] = 0;

        return *this;
    }

    // 正交投影
    Matrix4& InitOrtho(float left, float right, float bottom, float top, float near, float far)
    {
        float width = right - left;
        float height = top - bottom;
        float depth = far - near;

        m[0][0] = 2 / width;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = -(right + left) / width;
        m[1][0] = 0;
        m[1][1] = 2 / height;
        m[1][2] = 0;
        m[1][3] = -(top + bottom) / height;
        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = -2 / depth;
        m[2][3] = -(far + near) / depth;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this;
    }

    // 旋转矩阵
    Matrix4& InitRotation(const Vector4& forward, const Vector4& up)
    {
        Vector4 f = forward.Normal();

        Vector4 r = up.Normal();
        r = r.Cross(f);

        Vector4 u = f.Cross(r);

        return InitRotation(f, u, r);
    }

    // 旋转矩阵
    Matrix4& InitRotation(const Vector4& forward, const Vector4& up, const Vector4& right)
    {
        const Vector4& f = forward;
        const Vector4& r = right;
        const Vector4& u = up;

        m[0][0] = r.x;
        m[0][1] = r.y;
        m[0][2] = r.z;
        m[0][3] = 0;
        m[1][0] = u.x;
        m[1][1] = u.y;
        m[1][2] = u.z;
        m[1][3] = 0;
        m[2][0] = f.x;
        m[2][1] = f.y;
        m[2][2] = f.z;
        m[2][3] = 0;
        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

        return *this;
    }

    Vector4 Transform(const Vector4 r) const
    {
        return Vector4(m[0][0] * r.x + m[0][1] * r.y + m[0][2] * r.z + m[0][3] * r.w,
            m[1][0] * r.x + m[1][1] * r.y + m[1][2] * r.z + m[1][3] * r.w,
            m[2][0] * r.x + m[2][1] * r.y + m[2][2] * r.z + m[2][3] * r.w,
            m[3][0] * r.x + m[3][1] * r.y + m[3][2] * r.z + m[3][3] * r.w);
    }

    Matrix4 Mul(const Matrix4& r) const
    {
        Matrix4 res = Matrix4();

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res.Set(i, j,
                    m[i][0] * r[0][j] + m[i][1] * r[1][j] + m[i][2] * r[2][j] + m[i][3] * r[3][j]);
            }
        }

        return res;
    }

    Matrix4 operator*(const Matrix4& r) const
    {
        Matrix4 res = Matrix4();

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res.Set(i, j,
                    m[i][0] * r[0][j] + 
                    m[i][1] * r[1][j] + 
                    m[i][2] * r[2][j] + 
                    m[i][3] * r[3][j]);
            }
        }

        return res;
    }

    float Get(int x, int y) const
    {
        return m[x][y];
    }

    void Set(int x, int y, float v)
    {
        m[x][y] = v;
    }

    const float* operator[](int n) const
    {
        return m[n];
    }

    float* operator[](int n)
    {
        return m[n];
    }

private:
    float m[4][4];
};

typedef Matrix4 Matrix4f;

#endif