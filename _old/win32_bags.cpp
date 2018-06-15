
// #include "bags.cpp"

#include <windows.h>
#undef near
#undef far

#define WIN32_KEY_DOWN 0b1000000000000000

// global Input *GLOBAL_INPUT_P;
u16 system_keycodes[] = 
{
	0,
	VK_RETURN,
	VK_ESCAPE,
	VK_SPACE,
	VK_BACK,
	VK_DELETE,
	VK_LSHIFT,
	VK_RSHIFT,
	VK_LCONTROL,
	VK_RCONTROL,
	VK_LMENU,
	VK_RMENU,
	VK_LBUTTON,
	VK_MBUTTON,
	VK_RBUTTON,
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4D,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
};

struct System
{
	HWND window;
	HDC hdc;

	b32 resized;
	b32 fullscreen;
	b32 quit;

	Resolution last_resolution;
	Resolution resolution;
};
// global System *GLOBAL_SYSTEM_P;

struct Time
{
	r64 target_ms_per_frame;
	r64 ms_since_last_s;
	i64 perf_frequency;
	i64 frame_start;

	u32 last_frame_count;
	u32 frame_count;

	r32 dt;
};
// global Time *GLOBAL_TIME_P;

internal HWND
Win32CreateWindow(HINSTANCE instance, i32 cmd_show, WNDPROC WindowProcedure)
{
    HWND res;

	WNDCLASSEX wc;
	const char szClassName[] = "GameWindClass";
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wc.lpfnWndProc   = WindowProcedure;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = instance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm       = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
   										 IMAGE_ICON, 0, 0,
    									 LR_DEFAULTCOLOR|LR_SHARED|LR_DEFAULTSIZE);
    if(RegisterClassEx(&wc))
    {
    	u32 window_width = RESOLUTIONS[0].width;
    	u32 window_height = RESOLUTIONS[0].height;
    	res = CreateWindowEx(WS_EX_CLIENTEDGE,
					        szClassName,
					        GAME_TITLE,
					        WS_OVERLAPPEDWINDOW|CS_OWNDC,
					        0, 0, 
							window_width, window_height,
					        NULL, NULL, instance, NULL);
	    if(res != NULL)
	    {
			ShowWindow(res, cmd_show);
	    	
	    	return res;
	    }
	    else
			MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION|MB_OK);
    }
    else
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION|MB_OK);

    return 0;
}

internal void
Win32ResizeWindow(HWND window, u32 width, u32 height)
{
	SetWindowPos(window, HWND_TOP,
                   0, 0,
                   width, height,
                   SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

internal i64
CurrentTime ()
{
	LARGE_INTEGER res;
	QueryPerformanceCounter(&res);
	return res.QuadPart;
}

internal r64
TimeElapsed (Time *time, i64 a, i64 b)
{
	r64 res = (1000.0 * (b - a)) /
			  (r64) time->perf_frequency;
	return res;
}

internal void 
InitTime (Time *time, u32 target_fps)
{
	Time res = {};

	timeBeginPeriod(1);
	
	LARGE_INTEGER perf_frequency_i;
	QueryPerformanceFrequency(&perf_frequency_i);

	res.perf_frequency = perf_frequency_i.QuadPart;
	res.frame_start = CurrentTime();
	
	res.target_ms_per_frame = (1000.0/(r64)target_fps);

	*time = res;
}

internal void 
EndFrame (System *system, Time *time)
{
	SwapBuffers(system->hdc);
	UpdateWindow(system->window);

	++time->frame_count;
	i64 frame_end = CurrentTime();
	r64 ms_for_frame = TimeElapsed(time, time->frame_start,frame_end);

	time->dt = ((r32)ms_for_frame/1000.0f);
	
	if(ms_for_frame < time->target_ms_per_frame)
	{
		DWORD sleepms = time->target_ms_per_frame - ms_for_frame;
		Sleep(sleepms);
		ms_for_frame = TimeElapsed(time, time->frame_start, CurrentTime());
	}
	time->ms_since_last_s += ms_for_frame;
	if(time->ms_since_last_s >= 1000.0)
	{
		char str[100];
		wsprintf(str, "%s [%d FPS]", GAME_TITLE, time->last_frame_count);
		SetWindowText(system->window, str);

		time->ms_since_last_s = 0.0;
		time->last_frame_count = time->frame_count;
		time->frame_count = 0;
	}
	time->frame_start = frame_end;
}

internal void 
WatchKey (Input *input, u16 keycode)
{
	if(input->keys_on_watch_count <= MAX_KEYS_ON_WATCH)
	{
		KeyPress key = {};
		key.keycode = keycode;
		input->keys_on_watch[input->keys_on_watch_count++] = key;
	}
}

internal u16 
InternalToSystemKeyCode (u16 internal_keycode)
{
	u16 res = system_keycodes[internal_keycode];
	return res;
}

internal u16 
SystemToInternalKeyCode (u16 system_keycode)
{
	u16 res = 0;
	u32 count = sizeof(system_keycodes)/sizeof(u16);
	for (u32 i = 0; i < count; ++i)
	{
		if(system_keycodes[i] == system_keycode) 
		{
			res = i;
			break;
		}
	}
	return res;
}

internal void 
GetInput (System *system, Input *input, Time *time, MemoryPool *memory)
{
	if(input->keys_on_watch)
	{
		for (u16 i = 0; i < input->keys_on_watch_count; ++i)
		{
			KeyPress *key = input->keys_on_watch + i;
			b32 last = key->is_down;
			u16 system_keycode = InternalToSystemKeyCode(key->keycode);
			key->is_down = (((GetAsyncKeyState(system_keycode) & WIN32_KEY_DOWN) == WIN32_KEY_DOWN) ? true : false);
			if(last != key->is_down) 
				key->t = 0.0f;
			else key->t += time->dt;
		}
	}
	
	Pointer *pointer = input->first_pointer;
	while(pointer)
	{
		u8 type = POINTER_TYPE(pointer->flags);
		if(type==POINTER_TYPE_MOUSE) 
			break;
		pointer = pointer->next;
	}
	
	if(!pointer)
	{
		if(input->first_free_pointer)
		{
			pointer = input->first_free_pointer;
			input->first_free_pointer = input->first_free_pointer->next;
		}
		else
		{
			pointer = AllocateFromMemoryPool(memory, Pointer, 1);
		}

		pointer->next = input->first_pointer;
		input->first_pointer = pointer;
	}
		
	pointer->flags = POINTER_FLAGS(0,POINTER_TYPE_MOUSE,0);
	
	POINT mouse_p;
    GetCursorPos(&mouse_p);
    ScreenToClient(system->window, &mouse_p);
	pointer->screen_space_p = V2(mouse_p.x/* + (GLOBAL_RENDERERSTATE_P->top_x)*/,
								 mouse_p.y/* - GLOBAL_RENDERERSTATE_P->top_y*/);

	// Mat4 inv_mvp = Inverse(GLOBAL_RENDERERSTATE_P->gui_projection_matrix);
	// V4 normalized_screen_pointer = V4(((2.0f*((r32)(pointer->screen_space_p.x)/(GLOBAL_RENDERERSTATE_P->window_width)))-1.0f),
	// 					   				(1.0f-(2.0f*((r32)(pointer->screen_space_p.y)/(GLOBAL_RENDERERSTATE_P->window_height)))),
	// 					   				0.0f, 1.0f);
	// pointer->camera_space_p = (inv_mvp * normalized_screen_pointer * 3.0f).xy;

	// inv_mvp = Inverse(GLOBAL_RENDERERSTATE_P->world_view_matrix * GLOBAL_RENDERERSTATE_P->world_projection_matrix);
	// pointer->world_space_p = (inv_mvp * normalized_screen_pointer * 3.0f).xy + GLOBAL_RENDERERSTATE_P->camera.p.xy;
}

internal Pointer *
GetMousePointerP (Input *input)
{
	Pointer *res;
	
	Pointer *pointer = input->first_pointer;
	while(pointer)
	{
		if(POINTER_TYPE(pointer->flags)==POINTER_TYPE_MOUSE)
		{
			res = pointer;
			break;
		}
		pointer = pointer->next;
	}

	return res;
}

internal Pointer 
GetMousePointer (Input *input)
{
	Pointer res = *GetMousePointerP(input);
	return res;
}

internal b32 
IsKeyDown(Input *input, u16 keycode)
{
	b32 res = false;
	//TODO: binary search
	if(input->keys_on_watch)
	{
		for (u16 i = 0; i < input->keys_on_watch_count; ++i)
		{
			KeyPress *key = input->keys_on_watch + i;
			if(key->keycode == keycode)
			{
				res = key->is_down;
				break;
			}
		}
	}

	return res;
}

internal b32 
WasKeyJustPressed (Input *input, u16 keycode)
{
	b32 res = false;
	//TODO: binary search
	if(input->keys_on_watch)
	{
		for (u16 i = 0; i < input->keys_on_watch_count; ++i)
		{
			KeyPress *key = input->keys_on_watch + i;
			if(key->keycode == keycode)
			{
				res = (key->is_down && (key->t == 0.0f) ? true : false);
				break;
			}
		}
	}

	return res;
}

internal b32 
WasKeyJustReleased (Input *input, u16 keycode)
{
	b32 res = false;
	//TODO: binary search
	if(input->keys_on_watch)
	{
		for (u16 i = 0; i < input->keys_on_watch_count; ++i)
		{
			KeyPress *key = input->keys_on_watch + i;
			if(key->keycode == keycode)
			{
				res = (!key->is_down && (key->t == 0.0f) ? true : false);
				break;
			}
		}
	}

	return res;
}

internal void
SystemMessageLoop (System *system, Input *input)
{
	Pointer *mouse = GetMousePointerP(input);
	if(mouse) mouse->wheel = 0.0f;

	MSG msg;
	BOOL res;
	while((res = PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}



internal Resolution 
GetScreenResolution (System *system)
{
	Resolution res = {"screen"};

	MONITORINFO monitor_info = {sizeof(MONITORINFO)};
	GetMonitorInfo(MonitorFromWindow(system->window,MONITOR_DEFAULTTONEAREST), &monitor_info);
	RECT rect = monitor_info.rcMonitor;
	res.width = (rect.right - rect.left);
	res.height = (rect.bottom - rect.top);

	return res;
}

// #include "multimon.h"    

#define MONITOR_CENTER     0x0001        // center rect to monitor 
#define MONITOR_CLIP     0x0000        // clip rect to monitor 
#define MONITOR_WORKAREA 0x0002        // use monitor work area 
#define MONITOR_AREA     0x0000        // use monitor entire area 

internal void 
ClipOrCenterRectToMonitor(LPRECT prc, UINT flags)
{
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        rc;
    int         w = prc->right  - prc->left;
    int         h = prc->bottom - prc->top;

    // 
    // get the nearest monitor to the passed rect. 
    // 
    hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

    // 
    // get the work area or entire monitor rect. 
    // 
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    if (flags & MONITOR_WORKAREA)
        rc = mi.rcWork;
    else
        rc = mi.rcMonitor;

    // 
    // center or clip the passed rect to the monitor rect 
    // 
    if (flags & MONITOR_CENTER)
    {
        prc->left   = rc.left + (rc.right  - rc.left - w) / 2;
        prc->top    = rc.top  + (rc.bottom - rc.top  - h) / 2;
        prc->right  = prc->left + w;
        prc->bottom = prc->top  + h;
    }
    else
    {
        prc->left   = max(rc.left, min(rc.right-w,  prc->left));
        prc->top    = max(rc.top,  min(rc.bottom-h, prc->top));
        prc->right  = prc->left + w;
        prc->bottom = prc->top  + h;
    }
}

internal void 
ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags)
{
    RECT rc;
    GetWindowRect(hwnd, &rc);
    ClipOrCenterRectToMonitor(&rc, flags);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

internal void *
Allocate(u32 size)
{
	return VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

internal void
Deallocate(void *p, u32 size)
{
	VirtualFree(p, size, MEM_RELEASE);
}

internal RawFileContents 
GetRawFileContents (const char *filename, MemoryPool *memory)
{
	RawFileContents res = {};
	char txt[256];

	HANDLE file_handle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(file_handle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER file_size;
		if(GetFileSizeEx(file_handle, &file_size))
		{
			res.size = (u32)file_size.QuadPart;

			if(memory)
			{
				res.contents = AllocateFromMemoryPool_(memory, res.size);
			}
			else
				res.contents = VirtualAlloc(0, res.size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
			if(res.contents)
			{
				DWORD bytes_read;
				if(ReadFile(file_handle, res.contents, res.size, &bytes_read, 0))
				{
					// leu com sucesso
				}
				else
				{
					sprintf(txt, "Failed to read \"%s\"!", filename);
					MessageBox(NULL, txt, "Error!", MB_ICONEXCLAMATION|MB_OK);
					if(!memory)
						VirtualFree(res.contents, res.size, MEM_RELEASE);
				}
			}
			else
			{
				sprintf(txt, "Failed to alloc mem for \"%s\"!", filename);
				MessageBox(NULL, txt, "Error!", MB_ICONEXCLAMATION|MB_OK);
			} 
		}

		CloseHandle(file_handle);
	}
	else
	{
		sprintf(txt, "Failed to open \"%s\"!", filename);
		MessageBox(NULL, txt, "Error!", MB_ICONEXCLAMATION|MB_OK);
	}

	return res;
}

internal RawFileContents 
GetRawFileContents (const char *filename)
{
	RawFileContents res = GetRawFileContents (filename, 0);
	return res;
}

internal u8 *
GetTextFileContents (const char *filename, u32 encoding, MemoryPool *memory)
{
	RawFileContents rfc = GetRawFileContents (filename, memory);

	switch(encoding) // TODO: Checar validez / corrigir
	{
		default:
		case ASCII_encoding:
		{

		} break;
		case UTF_8_encoding:
		{

		} break;
	}

	return (u8*)rfc.contents;
}

internal u8 *
GetTextFileContents (const char *filename, u32 encoding)
{
	u8 *res = GetTextFileContents (filename, encoding, 0);
	return res;
}

internal b32
WriteFile (const char *path, u32 size, void *memory)
{
	b32 res = false;
	HANDLE file_handle = CreateFileA(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if(file_handle != INVALID_HANDLE_VALUE)
	{
		DWORD bytes_written;
		if(WriteFile(file_handle, memory, size, &bytes_written, 0))
		{
			res = (bytes_written == size) ? true : false;
		}
		else
		{
			// Nao escreveu
		}

		CloseHandle(file_handle);
	}
	else
	{
		// Nao criou
	}

	return res;
}

WINDOWPLACEMENT g_wpPrev = {sizeof(g_wpPrev)};
internal void 
ToggleFullscreen(System *system)
{
  DWORD dwStyle = GetWindowLong(system->window, GWL_STYLE);
  if (dwStyle & WS_OVERLAPPEDWINDOW) {
    MONITORINFO mi = { sizeof(mi) };
    if (GetWindowPlacement(system->window, &g_wpPrev) &&
        GetMonitorInfo(MonitorFromWindow(system->window,
                       MONITOR_DEFAULTTOPRIMARY), &mi)) {
      SetWindowLong(system->window, GWL_STYLE,
                    dwStyle & ~WS_OVERLAPPEDWINDOW);
      SetWindowPos(system->window, HWND_TOP,
                   mi.rcMonitor.left, mi.rcMonitor.top,
                   mi.rcMonitor.right - mi.rcMonitor.left,
                   mi.rcMonitor.bottom - mi.rcMonitor.top,
                   SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
  } else {
    SetWindowLong(system->window, GWL_STYLE,
                  dwStyle | WS_OVERLAPPEDWINDOW);
    SetWindowPlacement(system->window, &g_wpPrev);
    SetWindowPos(system->window, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
  }
  system->fullscreen = !system->fullscreen;
}

internal void 
ShowSystemPopupTextWindow(char *title, char *text)
{
	MessageBox(NULL, text, title, MB_ICONEXCLAMATION|MB_OK);
}

inline u32
FormatMemoryString (char *str, char *prefix, u32 bytes, char *postfix)
{
	u32 res = 0;
	
	u32 gb = ((bytes/1024)/1024)/1024;
	u32 mb = ((bytes/1024)/1024) - (gb*1024);
	u32 kb = (bytes/1024) - (mb*1024) - (gb*1024*1024);
	u32 b = bytes - KILOBYTES(kb) - MEGABYTES(mb) - GIGABYTES(gb);
	
	if(gb > 0)
		res = sprintf(str, "%s %dGb %dMb %dKb %db %s", prefix, gb,mb,kb,b, postfix);
	else if(mb > 0)
		res = sprintf(str, "%s %dMb %dKb %db %s", prefix, mb,kb,b, postfix);
	else if(kb > 0)
		res = sprintf(str, "%s %dKb %db %s", prefix, kb,b, postfix);
	else
		res = sprintf(str, "%s %db %s", prefix, b, postfix);

	return res;
}

inline u32
FormatTimeString (char *str, char *prefix, u32 milliseconds, char *postfix)
{
	u32 res = 0;

	u32 hours = ((milliseconds/1000)/60)/60;
	u32 minutes = ((milliseconds/1000)/60) - (hours*60);
	r32 seconds = ((r32)milliseconds)/1000.0f - (minutes*60) - (hours*60*60);

	if(hours > 0)
		res = sprintf(str, "%s %dh %dm %.2fs %s", prefix, hours,minutes,seconds, postfix);
	else if(minutes > 0)
		res = sprintf(str, "%s %dm %.2fs %s", prefix, minutes,seconds, postfix);
	else
		res = sprintf(str, "%s %.2fs %s", prefix, seconds, postfix);
	
	return res;
}

# ifdef OPENGL_RENDERER
	#include "..\..\BAGS\opengl_win32_bags.cpp"
#endif
# ifdef DIRECT_SOUND
	#include "..\..\BAGS\directsound_bags.cpp"
#endif
# ifdef DEBUG_BUILD
	#include "..\..\BAGS\editor_bags.cpp"
#endif
