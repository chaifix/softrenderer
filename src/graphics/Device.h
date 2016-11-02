#ifndef __DEVICE_H
#define __DEVICE_H
#include <SDL\SDL.h>
#include <iostream>
#include "Color.h"

class Device
{
public:
    static bool Init(int w, int h)
    {
        if (device == nullptr)
            device = new Device(w, h);

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1)
        {
            std::cerr << "Device::Init() abort: Init SDL failed." << std::endl;
            return false; 
        }
        SDL_WM_SetCaption("3D software render", 0);

        device->screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
        if (device->screen == nullptr)
        {
            std::cerr << "Device::Init() abort: Create windows surface failed." << std::endl; 
            return false;
        }

        return true;
    }
    static int GetWidth()
    {
        return device->w; 
    }
    static int GetHeight()
    {
        return device->h;
    }
    static void GetSize(int* w, int* h)
    {
        *w = device->w; 
        *h = device->h;
    }
    static void SwapBuffer()
    {
        SDL_Flip(device->screen);
    }
    static const Device* Get()
    {
        return device;
    }
private:
    static uint32* GetPixels()
    {
        return (uint32*)device->screen->pixels;
    }

private:
    static Device* device;

    Device(int _w, int _h) : w(_w), h(_h), screen(nullptr)
    {
    }
    ~Device()
    {
        SDL_free(screen);
        screen = nullptr; 
    }

private: 
    friend class Canvas; 
    SDL_Surface* screen;
    int w; 
    int h; 
};

#endif