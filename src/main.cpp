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
#include "graphics/Mesh.h"
#include "graphics/Camera.h"
using namespace std; 
extern const int screen_width = 500, screen_height = 400; 

int main(int args, char* argv[])
{
    Device::Init(screen_width, screen_height);

    Mesh mesh("res/monkey0.obj"); 
    Transform monkeyTransform = Transform(Vector4(0, 0.0f, 3.0f));

    Mesh terrain("res/terrain2.obj");
    Transform terrainTransform = Transform(Vector4(0, -1.0f, 0.0f));

    bool running = true; 
    SDL_Event e;
    // global canvas 
    Canvas g_canvas = Canvas (screen_width, screen_height);
    Bitmap texture("res/bricks2.jpg");
    Bitmap texture2("res/bricks.jpg");
    //Star3d s(4096, 64.0f, 20.0f);
    RenderContext rc(screen_width, screen_height); 
    rc.Clear();

    Camera camera = Camera(Matrix4f().InitPerspective(degreesToRadians(70.0f),
        (float)rc.GetWidth() / (float)rc.GetHeight(), 0.1f, 1000.0f));

    // 30fps
    unsigned int rate = 33;
    float delta = 1 / 30.f;
    float rotCounter = 0.0f;
    while (running)
    {
        rotCounter += 0.05;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false; 
            if (e.type == SDL_KEYDOWN)
            {
                camera.e = e; 
                camera.move = true;
            }
            else if (e.type == SDL_KEYUP)
            {
                camera.move = false; 
            }
        }
        if(camera.move) 
            camera.Update(camera.e, 1 / 30.f);
        monkeyTransform = monkeyTransform.Rotate(Quaternion(Vector4(0, 1, 0), delta));
        // clear canvas
        g_canvas.Clear();
        Matrix4 vp = camera.GetViewProjection();
        rc.ClearDepthBuffer();
        rc.Clear();
        rc.DrawMesh(mesh, vp, monkeyTransform.GetTransformation(), texture);
        rc.DrawMesh(terrain, vp, terrainTransform.GetTransformation(), texture2);

        rc.Render(g_canvas, 0, 0);
        //s.UpdateAndRender(g_canvas);

        // render canvas to screen
        g_canvas.Post(*Device::Get());
        Device::SwapBuffer();
        SDL_Delay(rate);
    }

    return 0;
}