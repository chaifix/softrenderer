#ifndef __BITMAP_H
#define __BITMAP_H
#include <string>
#include <iostream>
#include "../libs/stb/stb_image.h"
#include "Color.h"
#include "Drawable.h"

class Bitmap: Drawable
{
public:
    Bitmap(int _w, int _h)
    {
        w = _w; 
        h = _h; 
        pixels = new uint32[w*h];
        memset(pixels, 0, sizeof(uint32) * w * h);
    }
    Bitmap(std::string path)
    {
        //  A R G B
        uchar* components = stbi_load(path.c_str(), &w, &h, NULL, STBI_rgb_alpha);
        if (components == nullptr)
        {
            std::cerr << "Bitmap::Bitmap() abort: load image failed." << std::endl; 
            return;
        }
        pixels = (uint32*)components;
    }
    ~Bitmap()
    {
        stbi_image_free(pixels);
        pixels = nullptr; 
    }
    void Clear()
    {
        memset(pixels, 0, sizeof(uint32)*w*h);
    }
    int GetWidth() { return w; }
    int GetHeight() { return h; }
    uint32 GetPixel(int index) { return pixels[index]; }
    uint32 GetPixel(int x, int y) { return pixels[y*w + x]; }
    void Render(const Canvas& canvas, int x, int y); 
    void DrawPixel(int x, int y, uchar r, uchar g, uchar b, uchar a);
private:
    int w, h;
    uint32* pixels;
};

#endif