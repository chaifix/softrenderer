#ifndef __TRANSFORM_H
#define __TRANSFORM_H
#include "Vector4.h"
#include "Quaternion.h"

class Transform
{
public: 
    Transform(const Vector4& _pos = Vector4(0, 0, 0, 0))
        : pos(_pos), rot(0, 0, 0, 1), scale(1, 1, 1, 1)
    {
    }
    Transform(const Vector4& _pos, const Quaternion& _rot, const Vector4& _scale)
        : pos(_pos), rot(_rot), scale(_scale)
    {
    }
    Transform(const Transform& trans)
        :pos(trans.pos), rot(trans.rot), scale(trans.scale)
    {
    }
    Transform Rotate(const Quaternion& rotation)
    {
        return Transform(
            pos,
            rotation.Mul(rot).Normal(),
            scale
        );
    }

    Transform SetPos(const Vector4& pos)
    {
        return  Transform(pos, rot, scale);
    }

    Quaternion GetLookAtRotation(const Vector4& point, const Vector4& up)
    {
        return Quaternion(Matrix4f().InitRotation(point.Sub(pos).Normal(), up));
    }

    Transform LookAt(const Vector4& point, const Vector4 up)
    {
        return Rotate(GetLookAtRotation(point, up));
    }	
    Matrix4f GetTransformation()
    {
        Matrix4f translationMatrix = Matrix4f().InitTranslation(pos.GetX(), pos.GetY(), pos.GetZ());
        Matrix4f rotationMatrix = rot.ToRotationMatrix();
        Matrix4f scaleMatrix = Matrix4f().InitScale(scale.GetX(), scale.GetY(), scale.GetZ());

        return translationMatrix.Mul(rotationMatrix.Mul(scaleMatrix));
    }

public:
    Vector4 pos; 
    Quaternion rot; 
    Vector4 scale; 
};

#endif