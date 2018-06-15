
internal void DummyFunction() {}

internal HWND
Win32CreateWindow(const char *title, u32 window_width, u32 window_height, HINSTANCE instance, s32 cmd_show, WNDPROC WindowProcedure)
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
    wc.hIcon         = LoadIconA(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursorA(NULL, IDC_CROSS);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm       = (HICON)LoadImageA(NULL, MAKEINTRESOURCE(IDI_APPLICATION),
   										 IMAGE_ICON, 0, 0,
    									 LR_DEFAULTCOLOR|LR_SHARED|LR_DEFAULTSIZE);
    if(RegisterClassExA(&wc))
    {
    	res = CreateWindowExA(WS_EX_CLIENTEDGE,
					        szClassName,
					        title,
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
			MessageBoxA(NULL, "Window Creation Failed!", "Error!", MB_ICONERROR|MB_OK);
    }
    else
        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONERROR|MB_OK);

    return 0;
}

internal void *
GetFunctionAddress (HMODULE library, const char *name)
{
	void *p = (void *)wglGetProcAddress(name);

	if(p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1) )
	{
		p = (void *)GetProcAddress(library, name);
	}

	// if(!p) p = (void*)DummyFunction;

	return p;
}

internal void 
Win32Init()
{
	HMODULE user32 = LoadLibraryA("user32.dll");
	DefWindowProcA = (DEFWINDOWPROCA*)GetProcAddress(user32, "DefWindowProcA");
	GetDC = (GETDC*)GetProcAddress(user32, "GetDC");
	PeekMessageA = (PEEKMESSAGEA*)GetProcAddress(user32, "PeekMessageA");
	TranslateMessage = (TRANSLATEMESSAGE*)GetProcAddress(user32, "TranslateMessage");
	DispatchMessageA = (DISPATCHMESSAGEA*)GetProcAddress(user32, "DispatchMessageA");
	LoadIconA = (LOADICONA*)GetProcAddress(user32, "LoadIconA");
	LoadCursorA = (LOADCURSORA*)GetProcAddress(user32, "LoadCursorA");
	LoadImageA = (LOADIMAGEA*)GetProcAddress(user32, "LoadImageA");
	RegisterClassExA = (REGISTERCLASSEXA*)GetProcAddress(user32, "RegisterClassExA");
	CreateWindowExA = (CREATEWINDOWEXA*)GetProcAddress(user32, "CreateWindowExA");
	ShowWindow = (SHOWWINDOW*)GetProcAddress(user32, "ShowWindow");
	MessageBoxA = (MESSAGEBOXA*)GetProcAddress(user32, "MessageBoxA");
	GetAsyncKeyState = (GETASYNCKEYSTATE*)GetProcAddress(user32, "GetAsyncKeyState");
	// VirtualAlloc = (VIRTUALALLOC*)GetProcAddress(user32, "VirtualAlloc");
	// VirtualFree = (VIRTUALFREE*)GetProcAddress(user32, "VirtualFree");
	// QueryPerformanceCounter = (QUERYPERFORMANCECOUNTER*)GetProcAddress(user32, "QueryPerformanceCounter");
	// QueryPerformanceFrequency = (QUERYPERFORMANCEFREQUENCY*)GetProcAddress(user32, "QueryPerformanceFrequency");
	UpdateWindow = (UPDATEWINDOW*)GetProcAddress(user32, "UpdateWindow");
	GetWindowRect = (GETWINDOWRECT*)GetProcAddress(user32, "GetWindowRect");
	SetWindowTextA = (SETWINDOWTEXTA*)GetProcAddress(user32, "SetWindowTextA");
	GetSystemMetrics = (GETSYSTEMMETRICS*)GetProcAddress(user32, "GetSystemMetrics");
	GetCursorPos = (GETCURSORPOS*)GetProcAddress(user32, "GetCursorPos");
	ScreenToClient = (SCREENTOCLIENT*)GetProcAddress(user32, "ScreenToClient");
	
	HMODULE gdi32 = LoadLibraryA("gdi32.dll");
	SwapBuffers = (SWAPBUFFERS*)GetProcAddress(gdi32, "SwapBuffers");
	ChoosePixelFormat = (CHOOSEPIXELFORMAT*)GetProcAddress(gdi32, "ChoosePixelFormat");
	DescribePixelFormat = (DESCRIBEPIXELFORMAT*)GetProcAddress(gdi32, "DescribePixelFormat");
	SetPixelFormat = (SETPIXELFORMAT*)GetProcAddress(gdi32, "SetPixelFormat");

	HMODULE winmm = LoadLibraryA("winmm.dll");
	timeBeginPeriod = (TIMEBEGINPERIOD*)GetProcAddress(winmm, "timeBeginPeriod");

	HMODULE opengl32 = LoadLibraryA("opengl32.dll");
	wglGetProcAddress = (WGLGETPROCADDRESS*)GetProcAddress(opengl32, "wglGetProcAddress");
	wglCreateContext = (WGLCREATECONTEXT*)GetProcAddress(opengl32, "wglCreateContext");
	wglMakeCurrent = (WGLMAKECURRENT*)GetProcAddress(opengl32, "wglMakeCurrent");

	HMODULE shcore = LoadLibraryA("shcore.dll");
	SetProcessDpiAwareness = (SETPROCESSDPIAWARENESS*)GetProcAddress(shcore, "SetProcessDpiAwareness");
}

internal void 
PlatformGetInput(Input *input)
{
	if(input->first_watched_key)
	{
		WatchedKey *key = input->first_watched_key;
		while(key)
		{
			b32 pressed = ((key->state & KEY_STATE_DOWN) == KEY_STATE_DOWN) ? true : false;

			u16 system_keycode = InternalToSystemKeyCode(key->keycode);
			
			if((GetAsyncKeyState(system_keycode) & WIN32_KEY_DOWN) == WIN32_KEY_DOWN)
			{
				if(!pressed)
					key->state = KEY_STATE_CHANGED | KEY_STATE_DOWN;
				else
					key->state = KEY_STATE_DOWN;				
			}
			else 
			{
				if(pressed) 
					key->state = KEY_STATE_CHANGED;
				else
					key->state = 0;
			}

			key = key->next;
		}
	}
}


internal void *
PlatformAllocateMemory(u32 size)
{
	void *res = VirtualAlloc(0, size, (MEM_RESERVE|MEM_COMMIT), PAGE_READWRITE);
	return res;
}

internal void
PlatformDeallocateMemory_(void *memory, u32 size)
{
	VirtualFree(memory, size, MEM_RELEASE);
}

inline u64 
PlatformCurrentTimeMS (Time time)
{
	u64 res = 0;
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	res = (1000 * t.QuadPart) / time.clocks_per_ms;
	return res;
}

internal void 
PlatformInitTime (Time *time, u32 target_fps)
{
	Time res = {};

	timeBeginPeriod(1);
	
	LARGE_INTEGER perf_frequency_i;
	QueryPerformanceFrequency(&perf_frequency_i);

	res.clocks_per_ms = perf_frequency_i.QuadPart;
	
	res.target_ms_per_frame = (1000/target_fps);

	*time = res;

	time->frame_start = PlatformCurrentTimeMS(*time);
}

internal void
PlatformEndFrame(System *system, Time *time)
{
	Win32System *win32sys = (Win32System *)system->platform_specific;

	SwapBuffers(win32sys->hdc);
	UpdateWindow(win32sys->window);

	++time->frame_count;
	r64 frame_end = PlatformCurrentTimeMS(*time);
	r64 ms_for_frame = frame_end - time->frame_start;

	if(ms_for_frame < time->target_ms_per_frame)
	{
		DWORD sleepms = time->target_ms_per_frame - ms_for_frame;
		Sleep(sleepms);
		frame_end = PlatformCurrentTimeMS(*time);
		ms_for_frame = frame_end - time->frame_start;
	}

	time->dt = ((r32)ms_for_frame/1000.0f);
	
	time->ms_since_last_s += ms_for_frame;
	if(time->ms_since_last_s >= 1000)
	{
		time->ms_since_last_s = 0;
		time->last_frame_count = time->frame_count;
		time->frame_count = 0;
	}

	POINT cursor = {};
	GetCursorPos(&cursor);
	ScreenToClient(win32sys->window, &cursor);
	// s32 title_height = GetSystemMetrics(SM_CYCAPTION);

	char str[100];
	sprintf(str, "%s [%d FPS] [dt: %.3f] [%d %d]", GAME_TITLE, time->last_frame_count, time->dt, cursor.x,cursor.y);
	SetWindowTextA(win32sys->window, str);

	time->frame_start = frame_end;
}

internal RawFileContents 
PlatformGetRawFileContents (const char *filename, StaticMemory *memory)
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
				res.contents = AllocateStaticMemory_(memory, res.size);
			}
			else
				res.contents = (u8*)VirtualAlloc(0, res.size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
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
					MessageBoxA(NULL, txt, "Error!", MB_ICONEXCLAMATION|MB_OK);
					if(!memory)
						VirtualFree(res.contents, res.size, MEM_RELEASE);
				}
			}
			else
			{
				sprintf(txt, "Failed to alloc mem for \"%s\"!", filename);
				MessageBoxA(NULL, txt, "Error!", MB_ICONEXCLAMATION|MB_OK);
			} 
		}

		CloseHandle(file_handle);
	}
	else
	{
		sprintf(txt, "Failed to open \"%s\"!", filename);
		MessageBoxA(NULL, txt, "Error!", MB_ICONEXCLAMATION|MB_OK);
	}

	return res;
}

internal b32
PlaformWriteEntireFile (const char *filename, u8 *data, u32 size)
{
	b32 res = false;
	
	HANDLE file_handle = CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if(file_handle != INVALID_HANDLE_VALUE)
	{
		DWORD bytes_written;
		if(WriteFile(file_handle, (void*)data, size, &bytes_written, 0))
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
