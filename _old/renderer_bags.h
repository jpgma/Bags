
#define LINES 0
#define LINE_LOOP 1
#define LINE_STRIP 2
#define TRIANGLES 3
#define POINTS_ 4
#define TRIANGLE_STRIP 5
#define TRIANGLE_FAN 6

struct Camera
{
	V3 p;
	V3 focus;
	V3 up;

	V3 target;
};

// struct VECPath
// {
// // #if DEBUG_BUILD
// 	V2 *cbezier_points;
// 	u32 cbezier_point_count;
// // #endif

// 	u16 *tesselated_indices;
// 	u32 tesselated_index_count;
// 	V2 *tesselated_points;
// 	u32 tesselated_point_count;
	
// 	V4 bounds;

// 	VECPath *next;
// };

// struct VECShape
// {
// 	r32 z;
// 	V3 fill;
// 	V4 bounds;

// 	VECPath *paths;
// 	VECShape *next;
// };

// struct VECImage
// {
// 	r32 width, height;
// 	VECShape *shapes;
// };

#define VECIMAGE_FLAG_RASTERIZE 0b1
struct VECImage
{
	NSVGimage *nsvg_img;
	u32 width, height;
	u32 bitmap_id;
	u8 *bitmap;
	u32 flags;
};

struct RendererState
{
	b32 initialized;
	b32 ortho;

	u32 top_x;
	u32 top_y;
	u32 window_width;
	u32 window_height;
	r32 window_ratio;

	r32 min_y;
	r32 max_y;
	r32 near;
	r32 far;

	Mat4 world_view_matrix;
	Mat4 world_projection_matrix;
	Mat4 wvp;

	Mat4 gui_projection_matrix;
	Mat4 gui_view_matrix;
	
	Camera camera;
	r32 distance;

	V3 clear_color;

	FontBAG font_bag;
	Font debug_font;
	u32 debug_font_res;

	MemoryPool *temp_memory;

	MemoryPool bitmap_memory;
	NSVGrasterizer *rasterizer;

	void *renderer;
};
// global RendererState *GLOBAL_RENDERERSTATE_P;

internal void InitRenderer (System *system, RendererState *renderer_state, MemoryPool *memory, MemoryPool *temp_memory);

inline void RendererEnableDepth ();
inline void RendererDisableDepth ();
inline void RendererSetPointSize(r32 size);
inline void RendererSetLineWidth(r32 width);
internal void BeginRendering(RendererState *renderer_state);
internal void RendererResize (RendererState *renderer_state, u32 top_x, u32 top_y, u32 width, u32 height);
internal void RendererRGBABitmapToGPU (u32 *id, u32 width, u32 height, u8 *pixels);
internal void RendererSolidDataToGPU (RendererState *renderer_state, V3 *positions, V3 *colors, u32 count, u32 *points_id, u32 *positions_id, u32 *colors_id);

internal void RenderSolid (RendererState *renderer_state, Mat4 mvp, r32 alpha, u32 id, u32 count, u16 *indices, u32 mode);
internal void RenderSolidImmediate (RendererState *renderer_state, Mat4 mvp, r32 alpha, V3 *positions, V3 *colors, u32 count, u16 *indices, u32 index_count, u32 mode);
internal void RenderShapeImmediate (RendererState *renderer_state, Mat4 mvp, r32 alpha, Shape *shape);
internal void RenderBezierImmediate (RendererState *renderer_state, Mat4 mvp, r32 alpha, V3 p0, V3 p1, V3 p2, V3 p3, V3 color);
internal void RenderTexturedQuad (RendererState *renderer_state, Mat4 mvp, r32 alpha, V3 color, u32 texture_id, b32 outline);
internal V2 RenderText (RendererState *renderer_state, Font *font, Mat4 vp, char *text, u32 size, r32 wsize, V2 point, V3 color, r32 alpha, MemoryPool *memory, MemoryPool *temp_memory);

internal V3 ScreenSpaceToWorldSpace (RendererState *renderer_state, V2 screen_p, b32 fullscreen);
internal V2 WorldSpaceToScreenSpace (RendererState *renderer_state, V3 world_p, b32 fullscreen);
internal V2 ScreenSpaceToCameraSpace (RendererState *renderer_state, V2 screen_p, b32 fullscreen);
internal b32 PointInsideScaledRect (RendererState *renderer_state, V2 point, V3 center, V2 size);

internal void RenderVECImage (RendererState *renderer_state, V3 position, V2 size, Quat rotation, r32 alpha, VECImage *image, b32 fullscreen);

// internal void TesselateVECImage(VECImage *image, MemoryPool *memory);

struct Level;
internal void UpdateCameraSectionBound (Level *level, Camera *camera);

//((()+1)*257)>>16
#define UNPACK_COLOR_RBG(packed) V3(((((packed)&0xff)+1)*257)>>16,(((((packed)>>8)&0xff)+1)*257)>>16,(((((packed)>>16)&0xff)+1)*257)>>16)

global r32 rectangle_xy[] = 
{
	-0.5f,+0.5f, // tl
	-0.5f,-0.5f, // bl
	+0.5f,-0.5f, // br
	+0.5f,+0.5f  // tr
};
global r32 rectangle_uv[] = 
{
	0.0f, 1.0f, // tl
	0.0f, 0.0f, // bl
	1.0f, 0.0f, // br
	1.0f, 1.0f  // tr
};
global r32 rectangle_positions[] = 
{
	-0.5f,+0.5f, 0.0f, // tl
	-0.5f,-0.5f, 0.0f, // bl
	+0.5f,-0.5f, 0.0f, // br
	+0.5f,+0.5f, 0.0f  // tr	
};
global u16 rectangle_indices[] = {2,1,0, 0,3,2};
#define RECTANGLE_COLORS(color) \
{\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z\
}

global r32 box_positions[] = 
{
	-0.5f, -0.5f, +0.5f, // fbl
	+0.5f, -0.5f, +0.5f, // fbr
	-0.5f, +0.5f, +0.5f, // ftl
	+0.5f, +0.5f, +0.5f, // ftr
	-0.5f, -0.5f, -0.5f, // bbl
	+0.5f, -0.5f, -0.5f, // bbr
	-0.5f, +0.5f, -0.5f, // btl
	+0.5f, +0.5f, -0.5f, // btr
};
global u16 box_vertex_count = 8;
#define BOX_COLORS(color) \
{\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z,\
	(color).x,(color).y,(color).z\
}
global u16 box_wireframe_indices[] = 
{
	0,1,3,2,
	0,4,6,2,
	3,7,6,
	4,5,7,
	5,1
};
global u16 box_wireframe_count = 16;
global u32 box_wireframe_id;

#define SET_COLORS(color_array,color,count)\
for (u32 i = 0; i < count; ++i)\
{ (color_array)[i] = color; }

global r32 hexagon_positions[] = 
{
	-0.50f, 0.00f, 0.0f,
	-0.25f, 0.50f, 0.0f,
	 0.25f, 0.50f, 0.0f,
	 0.50f, 0.00f, 0.0f,
	 0.25f,-0.50f, 0.0f,
	-0.25f,-0.50f, 0.0f,
	0.0f,0.0f,0.0f
};
global u16 hexagon_indices[] = 
{
	6,0,1,
	1,0,3
};
global r32 hexagon_colors[] = 
{
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f
};

#define GUI_TR(ratio) V2( (ratio),  1.0f)
#define GUI_TL(ratio) V2(-(ratio),  1.0f)
#define GUI_BR(ratio) V2( (ratio), -1.0f)
#define GUI_BL(ratio) V2(-(ratio), -1.0f)
