#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#define NANOSVG_IMPLEMENTATION 1
#define NANOSVG_ALL_COLOR_KEYWORDS 1
#define NANOSVGRAST_IMPLEMENTATION 1
#include "nanosvg.h"
#include "nanosvgrast.h"

#include "..\..\..\definitions.h"
#include "..\..\..\math.h"
#include "..\..\..\platform.h"
#include "..\..\..\strings.cpp"
#include "..\..\..\win32.h"
#include "..\..\..\opengl.h"
#include "..\..\..\vecgraphs.h"
#include "..\..\..\svg.cpp"
#include "..\..\..\bmp.cpp"

#define GAME_TITLE "SVG"
#define TARGET_FPS 30

global System *SYSTEM;

LRESULT CALLBACK 
WindowProcedure(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case WM_ACTIVATE:
            if(SYSTEM)
            {
                if(wparam == WA_ACTIVE)
                    SYSTEM->window_active = true;
                else if(wparam == WA_INACTIVE)
                    SYSTEM->window_active = false;
            }
        break;
        
        case WM_SIZE: break;

        case WM_CLOSE:
        case WM_DESTROY: 
            SYSTEM->quit_requested = true;
        break;

        default: return DefWindowProcA(window, msg, wparam, lparam);
    }

    return 0;
}

s32 WINAPI
WinMain (HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd, s32 cmd_show)
{
    Win32Init();

    u32 w = 1280;
    u32 h = 720;
    HWND game_window = Win32CreateWindow(GAME_TITLE, w, h, instance, cmd_show, WindowProcedure);
    HDC hdc = GetDC(game_window);

    StaticMemory static_memory = {};
    u32 static_memory_size = sizeof(System) + sizeof(Win32System) + sizeof(Renderer) + 
                             sizeof(OpenGLAPI) + sizeof(Time) + MEGABYTES(1);
    InitializeStaticMemory(&static_memory, static_memory_size);

    SYSTEM = AllocateStaticMemory(&static_memory, 1, System);
    Win32System *win32sys = AllocateStaticMemory(&static_memory, 1, Win32System);
    Renderer *renderer = AllocateStaticMemory(&static_memory, 1, Renderer);
    OpenGLAPI *opengl_api = AllocateStaticMemory(&static_memory, 1, OpenGLAPI);
    Time *time = AllocateStaticMemory(&static_memory, 1, Time);

    win32sys->hdc = hdc;
    win32sys->window = game_window;
    SYSTEM->platform_specific = (void*)win32sys;
    SYSTEM->window_active = true;
    renderer->api_specific = (void*)opengl_api;

    PlatformInitTime(time, TARGET_FPS);
    InitOpenGLRenderer(SYSTEM, renderer);

    SolidShader solid_shader = opengl_api->solid_shader;

    const char *filename = "data\\donut.svg";
    NSVGimage *nsvg_image = nsvgParseFromFile(filename,"px",96);
    u32 *nsvg_path_ids;
    u32 *nsvg_path_counts;
    Mat4 nsvg_mvp;
    u32 nsvg_npaths = 0;
    {
        u32 npaths = 0;
        NSVGshape *s = nsvg_image->shapes;
        while(s)
        {
            NSVGpath *p = s->paths;
            while(p)
            {
                ++npaths;
                p = p->next;
            }
            s = s->next;
        }
        nsvg_path_ids = AllocateStaticMemory(&static_memory,npaths,u32);
        nsvg_path_counts = AllocateStaticMemory(&static_memory,npaths,u32);
        nsvg_npaths = npaths;

        u32 *pid = nsvg_path_ids;
        u32 *pct = nsvg_path_counts;
        s = nsvg_image->shapes;
        while(s)
        {
            NSVGpath *p = s->paths;
            while(p)
            {
                glGenVertexArrays(1, pid);
                u32 vbos[1];
                glGenBuffers(1, vbos);
                glUseProgram(solid_shader.id);
                glBindVertexArray(*pid);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
                glBufferData(GL_ARRAY_BUFFER, p->npts*2*sizeof(GLfloat), (r32*)p->pts, GL_STATIC_DRAW);
                glVertexAttribPointer(solid_shader.a_position, 2, GL_FLOAT, GL_FALSE, 0, 0); 
                glEnableVertexAttribArray(solid_shader.a_position);
                
                *pct++ = p->npts;
                
                p = p->next;
                ++pid;
            }
            s = s->next;
        }

        ApplyTransforms(&nsvg_mvp, V3(-1.0f,0.0f,0.0f), V3((1.0f/nsvg_image->width), (1.0f/nsvg_image->height), 1.0f), QUAT_ZERO);
    }

    VectorImage bags_image = VectorImageFromSVG(filename, 96.0f);
    u32 *bags_path_ids;
    u32 *bags_path_counts;
    u32 bags_npaths = 0;
    Mat4 bags_mvp;
    {
        u32 npaths = 0;
        VIPathGroup *g = bags_image.first_group;
        while(g)
        {
            VIPath *p = g->first_path;
            while(p)
            {
                ++npaths;
                p = p->next;
            }
            g = g->next;
        }
        bags_path_ids = AllocateStaticMemory(&static_memory,npaths,u32);
        bags_path_counts = AllocateStaticMemory(&static_memory,npaths,u32);
        bags_npaths = npaths;

        u32 *pid = bags_path_ids;
        u32 *pct = bags_path_counts;
        g = bags_image.first_group;
        while(g)
        {
            VIPath *p = g->first_path;
            while(p)
            {
                glGenVertexArrays(1, pid);
                u32 vbos[1];
                glGenBuffers(1, vbos);
                glUseProgram(solid_shader.id);
                glBindVertexArray(*pid);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
                glBufferData(GL_ARRAY_BUFFER, p->npoints*2*sizeof(GLfloat), (r32*)p->points, GL_STATIC_DRAW);
                glVertexAttribPointer(solid_shader.a_position, 2, GL_FLOAT, GL_FALSE, 0, 0); 
                glEnableVertexAttribArray(solid_shader.a_position);

                *pct++ = p->npoints;

                p = p->next;
                ++pid;
            }
            g = g->next;
        }

        ApplyTransforms(&bags_mvp, V3(+1.0f,0.0f,0.0f), V3((1.0f/bags_image.width), (1.0f/bags_image.height), 1.0f), QUAT_ZERO);
    }
    
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
        if(SYSTEM->quit_requested) running = false;

        if(SYSTEM->window_active)
        {
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            {
                s32 i = 0;
                NSVGshape *s = nsvg_image->shapes;
                while(s)
                {
                    NSVGpath *p = s->paths;
                    while(p)
                    {
                        RenderSolid(renderer, nsvg_mvp, V4(1.0f,0.3f,0.3f,1.0f), nsvg_path_ids[i], nsvg_path_counts[i], 0,0, RM_LINE_LOOP);
                        RenderSolid(renderer, nsvg_mvp, V4(1.0f,0.3f,0.3f,1.0f), nsvg_path_ids[i], nsvg_path_counts[i], 0,0, RM_POINTS);
                        const u32 res = 16;
                        V2 points[res];
                        for (s32 j = 0; j < p->npts-1; j+=3)
                        {
                            s32 k = 0;
                            r32 *ctrl = &p->pts[j*2];
                            points[k++] = V2(ctrl[0],ctrl[1]);
                            for (r32 t = (1.0f/res); t < 1.0f; t += (1.0f/res))
                            {
                                points[k++] = InterpolateCubicBezier2D(V2(ctrl[0],ctrl[1]),V2(ctrl[2],ctrl[3]),V2(ctrl[4],ctrl[5]),V2(ctrl[6],ctrl[7]), t);
                            }
                            k = k;
                        }
                        RenderSolidImmediate(renderer, nsvg_mvp, V4(1.0f,1.0f,1.0f,1.0f), (r32*)points, res, 0,0, RM_LINE_STRIP);
                        ++i;
                        p = p->next;
                    }
                    s = s->next;
                }
            }

            {
                s32 i = 0;
                VIPathGroup *g = bags_image.first_group;
                while(g)
                {
                    VIPath *p = g->first_path;
                    while(p)
                    {
                        RenderSolid(renderer, bags_mvp, V4(1.0f,1.0f,1.0f,1.0f), bags_path_ids[i], bags_path_counts[i], 0,0, RM_LINE_LOOP);
                        RenderSolid(renderer, bags_mvp, V4(1.0f,0.3f,0.3f,1.0f), bags_path_ids[i], bags_path_counts[i], 0,0, RM_POINTS);
                        ++i;
                        p = p->next;
                    }
                    g = g->next;
                }
            }

            PlatformEndFrame(SYSTEM, time);
        }
    }

    FreeStaticMemory(&static_memory);
    return 0;
}

#include "..\..\..\math.cpp"
#include "..\..\..\platform.cpp"
#include "..\..\..\vecgraphs.cpp"
#include "..\..\..\win32.cpp"
#include "..\..\..\opengl.cpp"