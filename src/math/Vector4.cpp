#include "Vector4.h"
#include "Quaternion.h"


Vector4 Vector4::Rotate(const Quaternion& rotation)
{
    Quaternion conjugate = rotation.Conjugate();

    Quaternion w = rotation.Mul(*this).Mul(conjugate);

    return Vector4(w.x, w.y, w.z, 1.0f);
}
