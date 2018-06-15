// #include <windows.h>
// #undef near
// #undef far

//////////////////////
/// VK CODES

#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */
#define VK_BACK           0x08
#define VK_TAB            0x09
#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14
#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19
#define VK_ESCAPE         0x1B
#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F
#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D
#define VK_SLEEP          0x5F
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91
#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC
#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7
#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US
#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xD

//////////////////////
/// WINDOW MESSAGES

#define WM_NULL                         0x0000
#define WM_CREATE                       0x0001
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_ACTIVATE                     0x0006
#define  WA_INACTIVE     0
#define  WA_ACTIVE       1
#define  WA_CLICKACTIVE  2
#define WM_SETFOCUS                     0x0007
#define WM_KILLFOCUS                    0x0008
#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#define WM_QUIT                         0x0012
#define WM_USER                         0x0400

//////////////////////
/// CLASS STYLES

#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_OWNDC            0x0020

//////////////////////
/// DATA TYPES

#if defined(_WIN64)
 typedef __int64 LONG_PTR; 
 typedef unsigned __int64 ULONG_PTR;
 typedef unsigned __int64 UINT_PTR;
#else
 typedef long LONG_PTR;
 typedef unsigned int UINT_PTR;
 typedef unsigned long ULONG_PTR;
#endif
#define CONST const
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long* LPDWORD;
typedef unsigned char BYTE;
typedef long long LONGLONG;
typedef unsigned int UINT;
typedef wchar_t WCHAR;
typedef short SHORT;
typedef long LONG;
typedef char CHAR;
typedef int BOOL;
typedef WORD ATOM;
typedef void* PVOID;
typedef void *LPVOID;
typedef PVOID HANDLE;
typedef HANDLE HGLRC;
typedef HANDLE HMENU;
typedef HANDLE HWND;
typedef HANDLE HDC;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;
typedef int MMRESULT;
typedef LONG_PTR LRESULT;
typedef ULONG_PTR SIZE_T;
typedef HANDLE HICON;
typedef HICON HCURSOR;
typedef HANDLE HBRUSH;
typedef LONG HRESULT;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef CHAR *LPSTR;
typedef CONST WCHAR *LPCWSTR;
typedef CONST CHAR *LPCSTR;
typedef WCHAR *LPWSTR;
typedef CONST void *LPCVOID;
#define CALLBACK __stdcall
#define WINAPI __stdcall
#ifdef UNICODE
 typedef LPCWSTR LPCTSTR; 
 #define MAKEINTRESOURCE MAKEINTRESOURCEW
#else
 typedef LPCSTR LPCTSTR;
 #define MAKEINTRESOURCE MAKEINTRESOURCEA
#endif
#define MAKEINTRESOURCEA(i) ((LPSTR)((ULONG_PTR)((WORD)(i))))
#define MAKEINTRESOURCEW(i) ((LPWSTR)((ULONG_PTR)((WORD)(i))))
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef int (WINAPI *PROC)();

//////////////////////
/// STANDART ICON IDs

#define IDI_APPLICATION     MAKEINTRESOURCE(32512)
#define IDI_HAND            MAKEINTRESOURCE(32513)
#define IDI_QUESTION        MAKEINTRESOURCE(32514)
#define IDI_EXCLAMATION     MAKEINTRESOURCE(32515)
#define IDI_ASTERISK        MAKEINTRESOURCE(32516)
#define IDI_WINLOGO         MAKEINTRESOURCE(32517)
#define IDI_SHIELD          MAKEINTRESOURCE(32518)
#define IDI_WARNING     	IDI_EXCLAMATION
#define IDI_ERROR       	IDI_HAND
#define IDI_INFORMATION 	IDI_ASTERISK

//////////////////////
/// STANDART CURSOR IDs

#define IDC_ARROW           MAKEINTRESOURCE(32512)
#define IDC_IBEAM           MAKEINTRESOURCE(32513)
#define IDC_WAIT            MAKEINTRESOURCE(32514)
#define IDC_CROSS           MAKEINTRESOURCE(32515)
#define IDC_UPARROW         MAKEINTRESOURCE(32516)
#define IDC_SIZENWSE        MAKEINTRESOURCE(32642)
#define IDC_SIZENESW        MAKEINTRESOURCE(32643)
#define IDC_SIZEWE          MAKEINTRESOURCE(32644)
#define IDC_SIZENS          MAKEINTRESOURCE(32645)
#define IDC_SIZEALL         MAKEINTRESOURCE(32646)
#define IDC_NO              MAKEINTRESOURCE(32648) /*not in win3.1 */
#define IDC_HAND            MAKEINTRESOURCE(32649)
#define IDC_APPSTARTING     MAKEINTRESOURCE(32650) /*not in win3.1 */
#define IDC_HELP            MAKEINTRESOURCE(32651)

//////////////////////
/// COLOR TYPES

#define COLOR_SCROLLBAR           0
#define COLOR_BACKGROUND          1
#define COLOR_ACTIVECAPTION       2
#define COLOR_INACTIVECAPTION     3
#define COLOR_MENU                4
#define COLOR_WINDOW              5
#define COLOR_WINDOWFRAME         6
#define COLOR_MENUTEXT            7
#define COLOR_WINDOWTEXT          8
#define COLOR_CAPTIONTEXT         9
#define COLOR_ACTIVEBORDER        10
#define COLOR_INACTIVEBORDER      11
#define COLOR_APPWORKSPACE        12
#define COLOR_HIGHLIGHT           13
#define COLOR_HIGHLIGHTTEXT       14
#define COLOR_BTNFACE             15
#define COLOR_BTNSHADOW           16
#define COLOR_GRAYTEXT            17
#define COLOR_BTNTEXT             18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT        20

//////////////////////
/// WINDOW STYLES

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L     /* WS_BORDER | WS_DLGFRAME  */
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_TILED            WS_OVERLAPPED
#define WS_ICONIC           WS_MINIMIZE
#define WS_SIZEBOX          WS_THICKFRAME
#define WS_TILEDWINDOW      WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)
#define WS_POPUPWINDOW      (WS_POPUP          | \
                             WS_BORDER         | \
                             WS_SYSMENU)
#define WS_CHILDWINDOW      (WS_CHILD)
#define WS_EX_DLGMODALFRAME     0x00000001L
#define WS_EX_NOPARENTNOTIFY    0x00000004L
#define WS_EX_TOPMOST           0x00000008L
#define WS_EX_ACCEPTFILES       0x00000010L
#define WS_EX_TRANSPARENT       0x00000020L
#define WS_EX_MDICHILD          0x00000040L
#define WS_EX_TOOLWINDOW        0x00000080L
#define WS_EX_WINDOWEDGE        0x00000100L
#define WS_EX_CLIENTEDGE        0x00000200L
#define WS_EX_CONTEXTHELP       0x00000400L


//////////////////////
/// PFD FLAGS

#define PFD_DOUBLEBUFFER            0x00000001
#define PFD_STEREO                  0x00000002
#define PFD_DRAW_TO_WINDOW          0x00000004
#define PFD_DRAW_TO_BITMAP          0x00000008
#define PFD_SUPPORT_GDI             0x00000010
#define PFD_SUPPORT_OPENGL          0x00000020
#define PFD_GENERIC_FORMAT          0x00000040
#define PFD_NEED_PALETTE            0x00000080
#define PFD_NEED_SYSTEM_PALETTE     0x00000100
#define PFD_SWAP_EXCHANGE           0x00000200
#define PFD_SWAP_COPY               0x00000400
#define PFD_SWAP_LAYER_BUFFERS      0x00000800
#define PFD_GENERIC_ACCELERATED     0x00001000
#define PFD_SUPPORT_DIRECTDRAW      0x00002000
#define PFD_DIRECT3D_ACCELERATED    0x00004000
#define PFD_SUPPORT_COMPOSITION     0x00008000

//////////////////////
/// MISC

#define IMAGE_BITMAP 0
#define IMAGE_ICON   1
#define IMAGE_CURSOR 2
#define LR_DEFAULTCOLOR     0x00000000
#define LR_MONOCHROME       0x00000001
#define LR_COLOR            0x00000002
#define LR_COPYRETURNORG    0x00000004
#define LR_COPYDELETEORG    0x00000008
#define LR_LOADFROMFILE     0x00000010
#define LR_LOADTRANSPARENT  0x00000020
#define LR_DEFAULTSIZE      0x00000040
#define LR_VGACOLOR         0x00000080
#define LR_LOADMAP3DCOLORS  0x00001000
#define LR_CREATEDIBSECTION 0x00002000
#define LR_COPYFROMRESOURCE 0x00004000
#define LR_SHARED           0x00008000
#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L
#define MB_CANCELTRYCONTINUE        0x00000006L
#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L
#define MB_USERICON                 0x00000080L
#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND
#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND
#define MEM_COMMIT     0x00001000
#define MEM_RESERVE    0x00002000
#define PAGE_READWRITE 0x04
#define MEM_RELEASE    0x8000
#define PFD_MAIN_PLANE     0
#define PFD_OVERLAY_PLANE  1
#define PFD_UNDERLAY_PLANE (-1)
#define SM_CYCAPTION 4
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define GENERIC_READ      0x80000000
#define GENERIC_WRITE     0x40000000
#define GENERIC_EXECUTE   0x20000000
#define GENERIC_ALL       0x10000000
#define FILE_SHARE_READ   0x00000001
#define FILE_SHARE_WRITE  0x00000002
#define FILE_SHARE_DELETE 0x00000004
#define CREATE_NEW        1
#define CREATE_ALWAYS     2
#define OPEN_EXISTING     3
#define OPEN_ALWAYS       4
#define TRUNCATE_EXISTING 5

//////////////////////
/// STRUCTURES

typedef union
{
	struct 
	{
		DWORD LowPart;
		LONG  HighPart;
	};
	struct 
	{
		DWORD LowPart;
		LONG  HighPart;
	} u;
	LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef struct 
{
	LONG x;
	LONG y;
} POINT, *PPOINT, *LPPOINT;

typedef struct 
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *LPRECT;

typedef struct 
{
	HWND   hwnd;
	UINT   message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD  time;
	POINT  pt;
} MSG, *PMSG, *LPMSG;
#define PM_NOREMOVE 0x0000
#define PM_REMOVE   0x0001
#define PM_NOYIELD  0x0002

typedef struct 
{
	UINT      cbSize;
	UINT      style;
	WNDPROC   lpfnWndProc;
	int       cbClsExtra;
	int       cbWndExtra;
	HINSTANCE hInstance;
	HICON     hIcon;
	HCURSOR   hCursor;
	HBRUSH    hbrBackground;
	LPCTSTR   lpszMenuName;
	LPCTSTR   lpszClassName;
	HICON     hIconSm;
} WNDCLASSEX, *PWNDCLASSEX;

typedef struct 
{
	WORD  nSize;
	WORD  nVersion;
	DWORD dwFlags;
	BYTE  iPixelType;
	BYTE  cColorBits;
	BYTE  cRedBits;
	BYTE  cRedShift;
	BYTE  cGreenBits;
	BYTE  cGreenShift;
	BYTE  cBlueBits;
	BYTE  cBlueShift;
	BYTE  cAlphaBits;
	BYTE  cAlphaShift;
	BYTE  cAccumBits;
	BYTE  cAccumRedBits;
	BYTE  cAccumGreenBits;
	BYTE  cAccumBlueBits;
	BYTE  cAccumAlphaBits;
	BYTE  cDepthBits;
	BYTE  cStencilBits;
	BYTE  cAuxBuffers;
	BYTE  iLayerType;
	BYTE  bReserved;
	DWORD dwLayerMask;
	DWORD dwVisibleMask;
	DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR, *PPIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

typedef struct
{
  DWORD  nLength;
  LPVOID lpSecurityDescriptor;
  BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED 
{
  ULONG_PTR Internal;
  ULONG_PTR InternalHigh;
  union 
  {
    struct 
    {
      DWORD Offset;
      DWORD OffsetHigh;
    };
    PVOID  Pointer;
  };
  HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;

//////////////////////
/// ENUMS

typedef enum
{ 
  PROCESS_DPI_UNAWARE           = 0,
  PROCESS_SYSTEM_DPI_AWARE      = 1,
  PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

//////////////////////
/// FUNCTIONS

typedef LRESULT WINAPI DEFWINDOWPROCA(HWND, UINT, WPARAM, LPARAM);
DEFWINDOWPROCA *DefWindowProcA;
typedef HDC GETDC(HWND);
GETDC *GetDC;
typedef BOOL WINAPI PEEKMESSAGEA(LPMSG, HWND, UINT, UINT, UINT);
PEEKMESSAGEA *PeekMessageA;
typedef BOOL WINAPI TRANSLATEMESSAGE(const MSG*);
TRANSLATEMESSAGE *TranslateMessage;
typedef LRESULT WINAPI DISPATCHMESSAGEA(const MSG *);
DISPATCHMESSAGEA *DispatchMessageA;
typedef HICON WINAPI LOADICONA(HINSTANCE, LPCTSTR);
LOADICONA *LoadIconA;
typedef HCURSOR WINAPI LOADCURSORA(HINSTANCE, LPCTSTR);
LOADCURSORA *LoadCursorA;
typedef HANDLE WINAPI LOADIMAGEA(HINSTANCE, LPCTSTR, UINT, int, int, UINT);
LOADIMAGEA *LoadImageA;
typedef ATOM WINAPI REGISTERCLASSEXA(const WNDCLASSEX *);
REGISTERCLASSEXA *RegisterClassExA;
typedef HWND WINAPI CREATEWINDOWEXA(DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
CREATEWINDOWEXA *CreateWindowExA;
typedef BOOL WINAPI SHOWWINDOW(HWND, int);
SHOWWINDOW *ShowWindow;
typedef int WINAPI MESSAGEBOXA(HWND, LPCTSTR, LPCTSTR, UINT);
MESSAGEBOXA *MessageBoxA;
typedef PVOID WINAPI WGLGETPROCADDRESS(LPCSTR);
WGLGETPROCADDRESS *wglGetProcAddress;
typedef SHORT WINAPI GETASYNCKEYSTATE(int);
GETASYNCKEYSTATE *GetAsyncKeyState;
typedef MMRESULT TIMEBEGINPERIOD(UINT);
TIMEBEGINPERIOD *timeBeginPeriod;
typedef BOOL WINAPI SWAPBUFFERS(HDC);
SWAPBUFFERS *SwapBuffers;
typedef BOOL UPDATEWINDOW(HWND);
UPDATEWINDOW *UpdateWindow;
typedef BOOL WINAPI SETWINDOWTEXTA(HWND, LPCTSTR);
SETWINDOWTEXTA *SetWindowTextA;
typedef int WINAPI CHOOSEPIXELFORMAT(HDC, const PIXELFORMATDESCRIPTOR *);
CHOOSEPIXELFORMAT *ChoosePixelFormat;
typedef int WINAPI DESCRIBEPIXELFORMAT(HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
DESCRIBEPIXELFORMAT *DescribePixelFormat;
typedef BOOL WINAPI SETPIXELFORMAT(HDC, int, const PIXELFORMATDESCRIPTOR *);
SETPIXELFORMAT *SetPixelFormat;
typedef BOOL WINAPI GETWINDOWRECT(HWND, LPRECT);
GETWINDOWRECT *GetWindowRect;
typedef int WINAPI GETSYSTEMMETRICS(int);
GETSYSTEMMETRICS *GetSystemMetrics;
typedef BOOL WINAPI GETCURSORPOS(LPPOINT);
GETCURSORPOS *GetCursorPos;
typedef BOOL SCREENTOCLIENT(HWND,LPPOINT);
SCREENTOCLIENT *ScreenToClient;
typedef HRESULT WINAPI SETPROCESSDPIAWARENESS(PROCESS_DPI_AWARENESS);
SETPROCESSDPIAWARENESS *SetProcessDpiAwareness;

extern "C"
{ // kernel32
	void *LoadLibraryA (const char *filename);
	void *GetProcAddress (void *library, const char *name);
	void Sleep (unsigned long ms);
	LPVOID VirtualAlloc(LPVOID address, SIZE_T size, DWORD alloc_type, DWORD protect);
	BOOL VirtualFree(LPVOID address, SIZE_T size, DWORD free_type);
	BOOL QueryPerformanceCounter(LARGE_INTEGER *perf_count);
	BOOL QueryPerformanceFrequency(LARGE_INTEGER *frequency);
	HANDLE WINAPI CreateFileA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	BOOL WINAPI WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	BOOL WINAPI ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
	BOOL WINAPI GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize);
	BOOL WINAPI CloseHandle(HANDLE hObject);
}

#define TRUE  1
#define FALSE 0
#define WIN32_KEY_DOWN 0b1000000000000000

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

struct Win32System
{
	HWND window;
	HDC hdc;
};

internal void Win32Init();
internal void *GetFunctionAddress (HMODULE library, const char *name);
internal HWND Win32CreateWindow(const char *title, u32 window_width, u32 window_height, HINSTANCE instance, s32 cmd_show, WNDPROC WindowProcedure);