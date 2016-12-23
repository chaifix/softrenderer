#include <iostream>
#include "graphics/Device.h"
#include "math/Vector4.h"
#include "graphics/Canvas.h"
#include "graphics/Bitmap.h"
#include "graphics/star3d.h"
#include "graphics/RenderContext.h"
#include "math/matrix4.h"
#include "math/Vertex2.h"
#include "math/util.h"
#include <iostream>
using namespace std; 
extern const int screen_width = 500, screen_height = 400; 

int main(int args, char* argv[])
{
    Matrix4 mat;
    mat.LoadIdentity();
    Matrix4 m2(mat);

    Device::Init(screen_width, screen_height);
    bool running = true; 
    SDL_Event e;
    // global canvas 
    Canvas g_canvas = Canvas (screen_width, screen_height);
    Bitmap img("res/bricks.jpg");
    Star3d s(4096, 64.0f, 20.0f);
    RenderContext rc(screen_width, screen_height); 
    rc.Clear();
    Vertex2 minYVert =  Vertex2( Vector4f(-1, -1, 0, 1),
         Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
    Vertex2 midYVert =  Vertex2( Vector4f(0, 1, 0, 1),
         Vector4f(0.5f, 1.0f, 0.0f, 0.0f));
    Vertex2 maxYVert =  Vertex2( Vector4f(1, -1, 0, 1),
         Vector4f(1.0f, 0.0f, 0.0f, 0.0f));

    Matrix4 projection = Matrix4().InitPerspective(degreesToRadians(70.0f),
        (float)rc.GetWidth() / (float)rc.GetHeight(), 0.1f, 1000.0f);
    // 30fps
    unsigned int rate = 33;
    float rotCounter = 0.0f;
    while (running)
    {
        rotCounter += 0.05; 
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false; 
        }
        // clear canvas
        g_canvas.Clear();

        Matrix4 translation = Matrix4().InitTranslation(0.0f, 0.0f, 3.0f);
        Matrix4 rotation = Matrix4().InitRotation(0.0f, rotCounter, 0.0f);
        Matrix4 transform = projection.Mul(translation.Mul(rotation));

        rc.Clear();
        rc.FillTriangle(maxYVert.Transform(transform),
            midYVert.Transform(transform), minYVert.Transform(transform), img);
        rc.Render(g_canvas, 0, 0);
        //s.UpdateAndRender(g_canvas);
        // render canvas to screen
        g_canvas.Post(*Device::Get());
        Device::SwapBuffer();
        SDL_Delay(rate);
    }

    return 0;
}