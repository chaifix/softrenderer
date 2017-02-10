#include "Bitmap.h"
#include "../math/util.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"
/**
(x, y) 
 +---------------------+
 |                     | 
 |                     | 
 |                     | 
 |                     | 
 |                     | 
 +---------------------+ (x + w - 1, y + h - 1) 
*/
void Bitmap::Render(const Canvas& canvas, int x, int y)
{
    int cw = canvas.GetWidth(); 
    int ch = canvas.GetHeight(); 
    if (x > cw - 1 ||  x + w <= 0 || y > ch - 1 || y + h <= 0)
        return;
    int left = max(x, 0); 
    int right = min(x + w - 1, cw - 1); 
    int top = max(y, 0); 
    int bottom = min(y + h - 1, ch - 1); 
    for(int _x = left ;_x <= right ;_x ++)
        for (int _y = top; _y <= bottom; _y++)
        {
            uint32 c = pixels[(_x - x) + (_y - y) * w];
            canvas.pixels[_x + _y * cw] = c ;
        }
}

void Bitmap::DrawPixel(int x, int y, uchar r, uchar g, uchar b, uchar a)
{
    int index = (x + y * w); 
    int _r = r << 16; 
    int _g = g << 8;
    int _b = b;
    int _a = a << 24;
    pixels[index] = _r | _g | _b | _a;
}

void Bitmap::CopyPixel(int destX, int destY, int srcX, int srcY, const Bitmap& src)
{
    int destIndex = (destX + destY * w) ;
    int srcIndex = (srcX + srcY * src.GetWidth());
    pixels[destIndex] = src.pixels[srcIndex];
}