
#define GUI_FLAG(x) (u32)(1<<(x))

#define GUI_ALLIGN_TOP              GUI_FLAG(1)
#define GUI_ALLIGN_BOTTOM           GUI_FLAG(2)
#define GUI_ALLIGN_VCENTER          GUI_FLAG(3)
#define GUI_ALLIGN_HCENTER          GUI_FLAG(4)
#define GUI_ALLIGN_ABOVE            GUI_FLAG(5)
#define GUI_ALLIGN_BELOW            GUI_FLAG(6)
#define GUI_ALLIGN_TO_RIGHT         GUI_FLAG(7)
#define GUI_ALLIGN_TO_LEFT          GUI_FLAG(8)
#define GUI_ALLIGN_AT_LEFT_BORDERS  GUI_FLAG(9)
#define GUI_ALLIGN_AT_RIGHT_BORDERS GUI_FLAG(10)

#define GUI_ITEM_WINDOW   GUI_FLAG(1)
#define GUI_ITEM_BUTTON   GUI_FLAG(2)
#define GUI_ITEM_SLIDER   GUI_FLAG(5)
#define GUI_ITEM_TEXTBOX  GUI_FLAG(4)
#define GUI_ITEM_CHECKBOX GUI_FLAG(3)
struct GUIItem
{
	u32 type;

	void *state;

	GUIItem *first_child;
	GUIItem *next;
};

struct GUISolidButton
{
	V2 p;
	V2 s;

	u32 point_count;
	u32 points_id;
	u32 colors_id;
	u32 mode;
};

struct GUIListItem
{
	char text[16];
	GUIListItem *next;
};

struct GUIList
{
	V2 p;
	V2 s;

	GUIListItem *first_item;
	GUIListItem *first_free_item;
};

#define GUI_MAX_WINDOW_TITLE_SIZE 64
#define GUI_WINDOW_TITLED      GUI_FLAG(0)
#define GUI_WINDOW_MOVABLE     GUI_FLAG(1)
#define GUI_WINDOW_SCALABLE    GUI_FLAG(2)
#define GUI_WINDOW_DETACHABLE  GUI_FLAG(3)
#define GUI_WINDOW_DETACHED    GUI_FLAG(4)
#define GUI_WINDOW_MINIMIZABLE GUI_FLAG(5)
#define GUI_WINDOW_MINIMIZED   GUI_FLAG(6)
#define GUI_WINDOW_CLOSABLE    GUI_FLAG(7)
#define GUI_WINDOW_CLOSED      GUI_FLAG(8)
#define GUI_WINDOW_SCROLLABLE  GUI_FLAG(9)
#define GUI_WINDOW_HIDE_SCROLL GUI_FLAG(10)
#define GUI_WINDOW_BORDERLESS  GUI_FLAG(11)
struct GUIWindow
{
	char title[GUI_MAX_WINDOW_TITLE_SIZE];
	
	u32 flags;
	r32 scroll_t;
	b32 maximized;
	b32 closed;

	V2 p;
	V2 s;
};

struct GUITheme
{
	V3 background_color;
	V3 primary_color;
	V3 secondary_color;
	V3 text_color;

	r32 alpha;
};

struct GUIState
{
	MemoryPool memory;

	GUITheme theme;

	GUIItem *first_item;
	GUIItem *first_free_item;

	V2 pointer;
	r32 pointer_t;
	b32 pointer_down;
	r32 pointer_scroll;
};

internal GUIWindow *GUI_CreateWindow (GUIState *gui_state, const char *label, u32 flags);
internal void GUI_Begin (GUIState *gui_state, Input *input);
internal void GUI_End (GUIState *gui_state);

internal void GUI_BeginWindow (GUIState *gui_state, GUIWindow *window);
internal b32 GUI_IsWindowAttached (GUIWindow *window);
internal void GUI_AttachWindow (GUIState *gui_state, GUIWindow *window, u32 flags);
internal void GUI_EndWindow (GUIState *gui_state);

internal void GUI_BeginList (GUIState *gui_state, GUIList *list, V2 list_size, V2 list_pad, u32 flags);
internal void GUI_AddToList (GUIState *gui_state, GUIListItem *item);
internal GUIListItem *GUI_EndList (GUIState *gui_state, GUIList *list);

internal GUISolidButton *GUI_CreateSolidButton (GUIState *gui_state, V3 *positions, u32 point_count, u32 mode);
internal b32 GUI_SolidButton (GUIState *gui_state, GUISolidButton *button, V2 button_size, V2 pad_size, u32 flags);