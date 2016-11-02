#ifndef __STAR3D_h
#define __STAR3D_h
#include <cmath>
#include "Canvas.h"

class Star3d
{
public:
    int spread; 
    int speed;
    int num; 
    float* starX;
    float* starY;
    float* starZ;
    Star3d(int num, float spread, float speed)
    {
        this->num = num; 
        this->spread = spread;
        this->speed = speed; 
        starX = new float[num];
        starY = new float[num];
        starZ = new float[num];
        for (int i = 0; i < num; i++)
            InitStar(i);
    }
    void InitStar(int i)
    {
        starX[i] = 2 * (std::rand() % 1000 / 1000.f - 0.5f) * spread; 
        starY[i] = 2 * (std::rand() % 1000 / 1000.f - 0.5f) * spread;
        starZ[i] = 2 * (std::rand() % 1000 / 1000.f + 0.00001f) * spread;

    }
    void UpdateAndRender(const Canvas& canvas)
    {
        float delta = 1 / 30.f;
        float halfWidth = canvas.GetWidth() / 2.f; 
        float halfHeight = canvas.GetHeight() / 2.f; 
        for (int i = 0; i < num; i++)
        {
            starZ[i] -= delta * speed; 
            if (starZ[i] <= 0)
            {
                InitStar(i); 
            }
            int x = (int)((starX[i] / starZ[i]) * halfWidth + halfWidth);
            int y = (int)((starY[i] / starZ[i]) * halfHeight + halfHeight);
            if (x < 0 || x >= canvas.GetWidth() ||
                (y < 0 || y >= canvas.GetHeight()))
            {
                InitStar(i);
            }
            else
            {
                canvas.pixels[x + y * canvas.GetWidth()] = 0xffffffff;
            }
        }
    }
};

#endif