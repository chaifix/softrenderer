#ifndef __CANVAS_H
#define __CANVAS_H
#include "Device.h"
typedef unsigned int uint32; 
class Canvas
{
public:
    Canvas() {};
    Canvas(unsigned int _w, unsigned int _h)
        : pixels(nullptr), w(_w), h(_h)
    {
        pixels = new uint32[w*h];
        memset(pixels, 0, sizeof(uint32) * w * h);
    }
    ~Canvas()
    {
        delete[] pixels; 
        pixels = nullptr; 
    }
    void Clear()
    {
        memset(pixels, 0, sizeof(uint32) * w * h);
    }
    unsigned int GetWidth() const { return w; }
    unsigned int GetHeight() const { return h; }
    // render canvas to screen 
    void Post(const Device& device)
    {
        if (device.GetWidth() < w || device.GetHeight() < h)
        {
            std::cerr << "Device::Post() abort: Canvas is too big." << std::endl;
            return;
        }
        memcpy(device.GetPixels(), pixels, w * h * sizeof(uint32));
    }
public:
    // A R G B 
    uint32* pixels; 
private:
    unsigned int w, h;
};
// global canvas 
extern Canvas g_canvas;
#endif