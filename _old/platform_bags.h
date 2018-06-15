

struct Time;
struct System;
struct MemoryPool;

struct Resolution
{
	char name[16];
	u16 width, height;
};
global Resolution RESOLUTIONS[] = 
{
	{"nHD",         640,360},
	{"qHD",         960,540},
	{"HD",          1280,720},
	{"WXGA",        1366,768},
	{"HD+",         1600,900},
	{"Full HD",     1920,1080},
	{"QHD",         2560,1440},
	{"QHD+",        3200,1800},
	{"4K UHD",      3840,2160},
	{"Full 4K UHD", 4096,2304},
	{"5K UHD",      5120,2880},
	{"8K UHD",      7680,4320},
	{"Full 8K UHD", 8192,4608}
};

struct KeyPress
{
	u16 keycode;
	b32 is_down;
	r32 t;
};
struct Pointer
{
	V2 screen_space_p;
	V2 camera_space_p;
	V2 world_space_p;

	u32 flags;
	r32 wheel;
	r32 t;

	Pointer *next;
};
struct Input
{
	KeyPress *keys_on_watch;
	u32 keys_on_watch_count;

	Pointer *first_pointer;
	Pointer *first_free_pointer;
};
#define MAX_KEYS_ON_WATCH (1<<8)

#define POINTER_TYPE_MOUSE 0x1
#define POINTER_TYPE_TOUCH 0x2
#define POINTER_FLAGS(down,type,id) (u32)(((down) ? 0b1000000000000000000000000 : 0)|\
((((u32)(type))<<16)&0b111111110000000000000000)|\
(((u32)(id))&0b1111111111111111))
#define IS_POINTER_DOWN(flags) (b32)(((flags)&0b1000000000000000000000000)==0b1000000000000000000000000)
#define POINTER_TYPE(flags) (u8)(((flags)>>16)&0b11111111)
#define POINTER_ID(flags) (u16)((flags)&0b1111111111111111)

struct RawFileContents
{
	void *contents;
	u32 size;
};

internal void InitTime (Time *time, u32 target_fps);
internal r64 TimeElapsed (Time *time, i64 a, i64 b);
internal i64 CurrentTime ();
internal void EndFrame (System *system, Time *time);

internal void InitInput (Input *input);
internal void WatchKey (Input *input, u16 keycode);
internal u16 InternalToSystemKeyCode (u16 internal_keycode);
internal u16 SystemToInternalKeyCode (u16 system_keycode);
internal void GetInput (System *system, Input *input, Time *time, MemoryPool *memory);
internal Pointer GetMousePointer (Input *input);
internal Pointer *GetMousePointerP (Input *input);
internal b32 IsKeyDown (Input *input, u16 keycode);
internal b32 WasKeyJustPressed (Input *input, u16 keycode);
internal b32 WasKeyJustReleased (Input *input, u16 keycode);

internal void SystemMessageLoop (System *system, Input *input);
internal void ToggleFullscreen (System *system);
internal void ShowSystemPopupTextWindow (char *title, char *text);
internal Resolution GetScreenResolution (System *system);

internal void *Allocate (u32 size);
internal void Deallocate(void *p, u32 size);

internal RawFileContents GetRawFileContents (const char *filename, MemoryPool *memory);
internal RawFileContents GetRawFileContents (const char *filename);
internal u8 *GetTextFileContents (const char *filename, u32 encoding, MemoryPool *memory);
internal u8 *GetTextFileContents (const char *filename, u32 encoding);
internal b32 WriteFile (const char *path, u32 size, void *memory);

inline u32 FormatMemoryString (char *str, char *prefix, u32 bytes, char *postfix);
inline u32 FormatTimeString (char *str, char *prefix, u32 milliseconds, char *postfix);

enum TextEcondings
{
	ASCII_encoding = 1,
	UTF_8_encoding,
};
enum KeyCodes
{
	KEY_ENTER = 1,
	KEY_ESC,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_DELETE,
	KEY_L_SHIFT,
	KEY_R_SHIFT,
	KEY_L_CTRL,
	KEY_R_CTRL,
	KEY_L_ALT,
	KEY_R_ALT,
	KEY_LEFT_MOUSE_BUTTON,
	KEY_MID_MOUSE_BUTTON,
	KEY_RIGHT_MOUSE_BUTTON,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_UP_ARROW,
	KEY_DOWN_ARROW,
	KEY_LEFT_ARROW,
	KEY_RIGHT_ARROW,
};