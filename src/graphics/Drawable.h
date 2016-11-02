#ifndef __DRAWABLE_H
#define __DRAWABLE_H
#include "Canvas.h"

class Drawable
{
public:
    virtual void Render(const Canvas& canvas, int x, int y) = 0;
};

#endif