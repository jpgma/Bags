#define NANOSVG_IMPLEMENTATION
#include "libs/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "libs/nanosvgrast.h"

#define MAX_FILE_PATH 64

struct AssetManagerState
{
	b32 initialized;

	char world_bag_path[MAX_FILE_PATH];
	char fonts_bag_path[MAX_FILE_PATH];
};
// global AssetManagerState *GLOBAL_ASSETMANAGERSTATE_P;


struct Path
{
	u32 fill_color;
	u32 outline_color;
	
	b32 closed;
	V2 close_point; // p/ divisoes em regions

	V2 *points;
	u32 point_count;
};

struct Shape
{
	NSVGshape *shape;

	V3 p;

	Path *paths;
	u32 path_count;

	u32 baked_bitmap_id;
	b32 baked;

// 	V3 *positions;
// 	V3 *colors;
// 	u32 point_count;
// 	u32 points_id;
// 	u32 positions_id;
// 	u32 colors_id;

// #if DEBUG_BUILD
// 	V3 *ctrl_positions;
// 	V3 *ctrl_colors;
// 	u32 ctrl_point_count;
// 	u32 ctrl_points_id;
// 	u32 ctrl_positions_id;
// 	u32 ctrl_colors_id;
// #endif
};

struct SGW;

internal void InitAssetManager (AssetManagerState *asset_state);
internal SGW *LoadEntireSGW (AssetManagerState *asset_state, MemoryPool *memory);