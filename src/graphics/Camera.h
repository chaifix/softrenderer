#ifndef __CAMERA_H
#define __CAMERA_H

#include <SDL/SDL.h>
#include "../math/Transform.h"

class Camera
{
public: 
    Camera(const Matrix4& projection)
        : projection(projection), 
          transform(), 
         move(false)
    {
        transform.pos.z = -2;
    }

    Matrix4 GetViewProjection()
    {
        Matrix4f cameraRotation = transform.rot.Conjugate().ToRotationMatrix();
        Vector4 cameraPos = transform.pos.Mul(-1);

        Matrix4f cameraTranslation = Matrix4f().InitTranslation(cameraPos.GetX(), cameraPos.GetY(), cameraPos.GetZ());

        return projection.Mul(cameraRotation.Mul(cameraTranslation));
    }

    void Update(const SDL_Event& e, float delta)
    {
        float sensitivityX = 2.66f * delta;
        float sensitivityY = 2.0f * delta;
        float movAmt = 5.0f * delta;

        static Vector4 Y_AXIS(0, 1, 0);

        switch (e.key.keysym.sym)
        {
        case SDLK_w: Move(transform.rot.GetForward(), movAmt); break;
        case SDLK_s: Move(transform.rot.GetForward(), -movAmt); break;
        case SDLK_a: Move(transform.rot.GetLeft(), movAmt); break;
        case SDLK_d: Move(transform.rot.GetRight(), movAmt); break;

        case SDLK_RIGHT: Rotate(Y_AXIS, sensitivityX); break;
        case SDLK_LEFT: Rotate(Y_AXIS, -sensitivityX); break;
        case SDLK_DOWN: Rotate(transform.rot.GetRight(), sensitivityY); break;
        case SDLK_UP: Rotate(transform.rot.GetRight(), -sensitivityY); break;
        }
    }

    void Move(const Vector4& dir, float amt)
    {
        transform = transform.SetPos(transform.pos.Add(dir.Mul(amt)));
    }

    void Rotate(const Vector4& axis, float angle)
    {
        transform = transform.Rotate(Quaternion(axis, angle));
        //transform.rot = Quaternion(axis, angle);
    }
public:
    Matrix4 projection; 
    Transform transform;
    bool move; 
    SDL_Event e;
};

#endif