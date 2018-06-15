#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "..\..\..\definitions.h"
#include "..\..\..\math.h"

struct Level2DPlane
{
    V2 c;
    V2 dim;

    u8 *pixel_data;
    u32 pixel_width,pixel_height;
    u32 texture_id;

    Level2DPlane *next;
};

struct Level2D
{
    u32 width, height;

    Level2DPlane *foreground;
    Level2DPlane *background;
};

inline u32
IndexLRTBtoLRBT (u32 x, u32 y, u32 width, u32 height)
{
    u32 res = (x) + ((y)*width);
    return res;
}

#include "..\..\..\platform.h"
#include "..\..\..\strings.cpp"
#include "..\..\..\win32.h"
#include "..\..\..\opengl.h"
#include "..\..\..\xcf.cpp"

#define GAME_TITLE "XCF TEST"

internal void 
FreeLevel (Level2D *level)
{
    Level2DPlane *plane = level->background;
    while(plane)
    {
        if(plane->pixel_data)
            free(plane->pixel_data);
        Level2DPlane *last = plane;
        plane = plane->next;
        free(last);
    }

    plane = level->foreground;
    while(plane)
    {
        if(plane->pixel_data)
            free(plane->pixel_data);
        Level2DPlane *last = plane;
        plane = plane->next;
        free(last);
    }
}

internal void
Level2DToGPU (Level2D *level)
{
    Level2DPlane *plane = level->background;
    while(plane)
    {
        BitmapToGPU(&plane->texture_id, plane->pixel_data, plane->pixel_width, plane->pixel_height);
        plane = plane->next;
    }

    plane = level->foreground;
    while(plane)
    {
        BitmapToGPU(&plane->texture_id, plane->pixel_data, plane->pixel_width, plane->pixel_height);
        plane = plane->next;
    }
}

// void main()
// {
//     RawFileContents xcf = PlatformGetRawFileContents("data\\level_00.xcf", 0);
//     Level2D level = Level2DFromXCF(xcf.contents, xcf.size);
// }

global b32 window_active;
global b32 quit_requested;

LRESULT CALLBACK 
WindowProcedure(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case WM_ACTIVATE:
            if(wparam == WA_ACTIVE)
                window_active = true;
            else if(wparam == WA_INACTIVE)
                window_active = false;
        break;
        
        case WM_SIZE: break;

        case WM_CLOSE:
        case WM_DESTROY: 
            quit_requested = true;
        break;

        default: return DefWindowProcA(window, msg, wparam, lparam);
    }

    return 0;
}

s32 WINAPI
WinMain (HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd, s32 cmd_show)
{
    const char *filename = "data\\level_00.xcf";
    // const char *filename = "data\\test.xcf";
    // const char *filename = "data\\test01.xcf";
    RawFileContents xcf = {};
    Level2D level = {};

    Win32Init();

    r32 s = 1.5f;
    HWND window = Win32CreateWindow(GAME_TITLE, 800*s, 450*s, instance, cmd_show, WindowProcedure);
    HDC hdc = GetDC(window);

    System system = {};
    Win32System win32sys = {};
    Renderer renderer = {};
    OpenGLAPI opengl_api = {};
    Input input = {};
    Time time = {};
    PlatformInitTime(&time, 30);

    win32sys.hdc = hdc;
    win32sys.window = window;
    system.platform_specific = (void*)&win32sys;
    system.window_active = true;
    renderer.api_specific = (void*)&opengl_api;

    InitOpenGLRenderer(&system, &renderer);

    TextureShader texture_shader = {};
    OpenGLInitTextureShader(&texture_shader);

    u32 texture_id = 0;
    u32 pixel_width = 2;
    u32 pixel_height = 2;
    u8 *pixel_data = (u8*)calloc((pixel_width*pixel_height),sizeof(u32));
    for (s32 y = 0; y < pixel_height; ++y)
    {
        for (s32 x = 0; x < pixel_width; ++x)
        {
            ((u32*)pixel_data)[x+(y*pixel_width)] = 0xff000000;
        }
    }
    ((u32*)pixel_data)[0] = 0xffff0000;
    ((u32*)pixel_data)[1] = 0xff00ff00;
    ((u32*)pixel_data)[2] = 0xff0000ff;
    ((u32*)pixel_data)[3] = 0xffff00ff;
    BitmapToGPU(&texture_id, pixel_data, pixel_width, pixel_height);
    free(pixel_data);

    StaticMemory static_memory = {};
    InitializeStaticMemory(&static_memory, MEGABYTES(1));
    WatchKey(&input, &static_memory, IK_F5);
    WatchKey(&input, &static_memory, IK_LCONTROL);
    WatchKey(&input, &static_memory, IK_DOWN);
    WatchKey(&input, &static_memory, IK_UP);

    b32 running = true;
    while(running)
    {
        MSG msg;
        BOOL res;
        while((res = PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if(quit_requested) running = false;


        if(window_active)
        {
            PlatformGetInput(&input);

            if(WasKeyJustReleased(&input, IK_F5) || !xcf.contents)
            {
                FreeLevel(&level);
                PlatformDeallocateMemory(xcf.contents,xcf.size);

                xcf = PlatformGetRawFileContents(filename, 0);
                level = Level2DFromXCF(xcf.contents, xcf.size);
                Level2DToGPU(&level);
            }

            glClearColor(0.3f,0.3f,0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Mat4 model = Mat4Identity;
            Mat4 mvp = Mat4Identity;

            static r32 scale = 4.0f;
            if(IsKeyDown(&input, IK_LCONTROL)&&IsKeyDown(&input, IK_UP)) scale += 2.0f*time.dt;
            if(IsKeyDown(&input, IK_LCONTROL)&&IsKeyDown(&input, IK_DOWN)) scale -= 2.0f*time.dt;
            r32 ratio = ((r32)level.width)/level.height;
            {
                ApplyTransforms(&model, V3(0.0f,0.0f,0.0f), V3(ratio,1.0f,1.0f)*scale, QUAT_ZERO);
                mvp = (model * renderer.view) * renderer.projection;
                glLineWidth(5.0f);
                RenderSolidImmediate(&renderer, mvp, V4(1.0f,1.0f,1.0f,1.0f), (r32*)rectangle_positions, 4, 0,0, RM_LINE_LOOP);
                glLineWidth(1.5f);
            }

            Level2DPlane *plane = level.background;
            while(plane)
            {
                ApplyTransforms(&model, V3((plane->c.x-(plane->dim.x*0.5f))*scale,(plane->c.y-(plane->dim.y*0.5f))*scale,0.001f), V3(plane->dim.x,plane->dim.y,1.0f)*scale, QUAT_ZERO);
                mvp = (model * renderer.view) * renderer.projection;
                RenderTexturedQuad(texture_shader, mvp, plane->texture_id, 1.0f);

                ApplyTransforms(&model, V3(plane->c.x*scale,plane->c.y*scale,0.001f), V3(plane->dim.x,plane->dim.y,1.0f)*scale, QUAT_ZERO);
                mvp = (model * renderer.view) * renderer.projection;
                RenderSolidImmediate(&renderer, mvp, V4(0.3f,0.3f,0.8f,1.0f), (r32*)rectangle_positions, 4, 0,0, RM_LINE_LOOP);
                plane = plane->next;
            }

            plane = level.foreground;
            while(plane)
            {
                ApplyTransforms(&model, V3((plane->c.x-(plane->dim.x*0.5f))*scale,(plane->c.y-(plane->dim.y*0.5f))*scale,0.0015f), V3(plane->dim.x,plane->dim.y,1.0f)*scale, QUAT_ZERO);
                mvp = (model * renderer.view) * renderer.projection;
                RenderTexturedQuad(texture_shader, mvp, plane->texture_id, 1.0f);

                ApplyTransforms(&model, V3(plane->c.x*scale,plane->c.y*scale,0.0015f), V3(plane->dim.x,plane->dim.y,1.0f)*scale, QUAT_ZERO);
                mvp = (model * renderer.view) * renderer.projection;
                RenderSolidImmediate(&renderer, mvp, V4(0.3f,0.3f,0.8f,1.0f), (r32*)rectangle_positions, 4, 0,0, RM_LINE_LOOP);
                plane = plane->next;
            }
                        

            ApplyTransforms(&model, V3(-0.5f,-0.5f,0.003f), V3(1.0f,1.0f,1.0f), QUAT_ZERO);
            mvp = (model * renderer.view) * renderer.projection;
            // RenderTexturedQuad(texture_shader, mvp, texture_id, 1.0f);
            // RenderSolid(&renderer, mvp, V4(1.0f,1.0f,1.0f,1.0f), texture_shader.quad_vao, 4, 0,0, RM_LINE_LOOP);

            PlatformEndFrame(&system, &time);
        }
    }

    PlatformDeallocateMemory(xcf.contents,xcf.size);
    FreeStaticMemory(&static_memory);
    FreeLevel(&level);
    return 0;
}

#include "..\..\..\math.cpp"
#include "..\..\..\platform.cpp"
#include "..\..\..\win32.cpp"
#include "..\..\..\opengl.cpp"
