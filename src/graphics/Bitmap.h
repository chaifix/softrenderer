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
        for (int _x = 0; _x <= w; _x++)
            for (int _y = 0; _y <= h; _y++)
            {
                uint32 c = pixels[(_x) + (_y) * w];
                uint32 r = (c & 0xff) << 16;
                uint32 g = (c & 0xff00);
                uint32 b = (c & 0xff0000) >> 16;
                c = r | g | b | 0xff000000;
                pixels[_x + _y * w] = c;
            }
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
    void CopyPixel(int desX, int desY, int srcX, int srcY, const Bitmap& src);
    int GetWidth() const { return w; }
    int GetHeight() const { return h; }
    uint32 GetPixel(int index) { return pixels[index]; }
    uint32 GetPixel(int x, int y) { return pixels[y*w + x]; }
    void Render(const Canvas& canvas, int x, int y); 
    void DrawPixel(int x, int y, uchar r, uchar g, uchar b, uchar a);
private:
    int w, h;
    uint32* pixels;
};

#endif