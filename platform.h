
#define KILOBYTES(x) ((x)*1024)
#define MEGABYTES(x) (KILOBYTES(x)*1024)
#define GIGABYTES(x) (MEGABYTES(x)*1024)

struct StaticMemory
{
	u8 *base;
	u32 size;
	u32 used;
};

internal void InitializeStaticMemory(StaticMemory *memory, u32 size);
internal u8 *AllocateStaticMemory_(StaticMemory *memory, u32 size);
#define AllocateStaticMemory(memory,count,type) ((type*)AllocateStaticMemory_((memory),((count)*sizeof(type))))
internal void FreeStaticMemory(StaticMemory *memory);

struct System
{
	b32 quit_requested;
	b32 window_resized;
	b32 window_active;
	void *platform_specific;
};

struct FileStream
{
    u8 *data;
    u32 size;
    u32 offset;
};

inline void *ConsumeSize(FileStream *stream, u32 size);
#define Consume(stream,type) (type*)ConsumeSize((stream),sizeof(type))

struct RawFileContents
{
	u32 size;
	u8 *contents;
};

internal RawFileContents PlatformGetRawFileContents(const char *filename, StaticMemory *memory);
internal b32 PlaformWriteEntireFile(const char *filename, u8 *data, u32 size);

struct Renderer
{
	b32 initialized;
	void *api_specific;
	s32 window_width, window_height;
	Mat4 view, projection, gui_projection;
};

struct Bitmap
{
	u32 width,height;
	u8 *pixels;
};

internal void RenderSolid (Renderer *renderer, Mat4 mvp, V4 color, u32 id, u32 count, u16 *indices, u16 index_count, u32 type);
internal void RenderSolidImmediate (Renderer *renderer, Mat4 mvp, V4 color, r32 *positions, u32 count, u16 *indices, u16 index_count, u32 mode);

struct Time
{
	u32 frame_count;
	u32 last_frame_count;

	u64 frame_start;
	u64 ms_since_last_s;
	u64 target_ms_per_frame;
	u64 clocks_per_ms;

	r32 dt;
};

#define KEY_STATE_DOWN 0b1
#define KEY_STATE_CHANGED 0b10

struct WatchedKey
{
	u16 keycode;
	u16 state;

	WatchedKey *next;
};

struct Input
{
	WatchedKey *first_watched_key;
};
internal void WatchKey (Input *input, StaticMemory *memory, u16 keycode);
inline b32 IsKeyDown(Input *input, u16 internal_keycode);
inline b32 WasKeyJustPressed(Input *input, u16 internal_keycode);
inline b32 WasKeyJustReleased(Input *input, u16 internal_keycode);

internal void PlatformInitTime (Time *time, u32 target_fps);
internal void *PlatformAllocateMemory(u32 size);
internal void PlatformDeallocateMemory_(void *memory, u32 size);
#define PlatformDeallocateMemory(memory,size) PlatformDeallocateMemory_((void*)(memory),(size))
inline u64 PlatformCurrentTimeMS (Time time);
internal void PlatformEndFrame(System *system, Time *time);
internal void PlatformGetInput(Input *input);

enum InternalKeycodes
{
	IK_NULL,
	IK_RETURN,
	IK_ESCAPE,
	IK_SPACE,
	IK_BACK,
	IK_DELETE,
	IK_LSHIFT,
	IK_RSHIFT,
	IK_LCONTROL,
	IK_RCONTROL,
	IK_LMENU,
	IK_RMENU,
	IK_LBUTTON,
	IK_MBUTTON,
	IK_RBUTTON,
	IK_0,
	IK_1,
	IK_2,
	IK_3,
	IK_4,
	IK_5,
	IK_6,
	IK_7,
	IK_8,
	IK_9,
	IK_F1,
	IK_F2,
	IK_F3,
	IK_F4,
	IK_F5,
	IK_F6,
	IK_F7,
	IK_F8,
	IK_F9,
	IK_F10,
	IK_F11,
	IK_F12,
	IK_A,
	IK_B,
	IK_C,
	IK_D,
	IK_E,
	IK_F,
	IK_G,
	IK_H,
	IK_I,
	IK_J,
	IK_K,
	IK_L,
	IK_M,
	IK_N,
	IK_O,
	IK_P,
	IK_Q,
	IK_R,
	IK_S,
	IK_T,
	IK_U,
	IK_V,
	IK_W,
	IK_X,
	IK_Y,
	IK_Z,
	IK_UP,
	IK_DOWN,
	IK_LEFT,
	IK_RIGHT,
};

enum RenderModes
{
	RM_LINES,
	RM_LINE_LOOP,
	RM_LINE_STRIP,
	RM_TRIANGLES,
	RM_POINTS,
	RM_TRIANGLE_STRIP,
	RM_TRIANGLE_FAN,
};

global const r32 rectangle_positions[] = 
{
	-0.5f,+0.5f,/* 0.0f,*/ // tl
	-0.5f,-0.5f,/* 0.0f,*/ // bl
	+0.5f,-0.5f,/* 0.0f,*/ // br
	+0.5f,+0.5f,/* 0.0f*/  // tr	
};
global const r32 rectangle_uv[] = 
{
	+0.0f,+1.0f,/* 0.0f,*/ // tl
	+0.0f,+0.0f,/* 0.0f,*/ // bl
	+1.0f,+0.0f,/* 0.0f*/  // br	
	+1.0f,+1.0f,/* 0.0f,*/ // tr
};
global const u16 rectangle_indices[] = {0,1,2, 2,3,0};