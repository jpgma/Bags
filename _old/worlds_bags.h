
//
// Sparse Grid World //////////////////////////////////////////////////////////
//

#define SGWREGION_WIDTH (16.0f)
#define SGWREGION_HEIGHT (9.0f)
#define SGWREGION_DEPTH (3.0f)

#define SGWREGION_LEFT (SGWREGION_WIDTH * -0.5f)
#define SGWREGION_RIGHT (SGWREGION_WIDTH * +0.5f)
#define SGWREGION_BOTTOM (SGWREGION_HEIGHT * -0.5f)
#define SGWREGION_TOP (SGWREGION_HEIGHT * +0.5f)
#define SGWREGION_BACK (SGWREGION_DEPTH * -0.5f)
#define SGWREGION_FRONT (SGWREGION_DEPTH * +0.5f)

#define SGW_GRAVITYTYPE_LOCAL 0x1
#define SGW_GRAVITYTYPE_WORLD 0x2

struct SGWPosition
{
	i32 x,y,z;
};

struct CollisionPath
{
	V2 *points;
	u32 point_count;
	CollisionPath *next;
};

struct SGWRegion;
struct SGWRegionGroupShapes
{
	SGWRegion *min;
	SGWRegion *max;

	V2 center;
	r32 width, height;

	VECImage image;
};

struct SGWRegion
{
	SGWPosition p;
	CollisionPath *collision_paths;

	SGWRegionGroupShapes *group;
};

global V2 SGWREGION_SIZE = V2(SGWREGION_WIDTH,SGWREGION_HEIGHT);

#define SGWREGIONBLOCK_KEY(wp) (u64)(((u64)((SGWPosition)(wp)).x)<<32)|((u64)((SGWPosition)(wp)).y)
struct SGWRegionBlock
{
	u64 key;

	SGWRegion *regions;
	u32 region_count;
};

struct SGW
{
	b32 loaded;
	u32 width, height, depth; // em num de regions
	r32 abs_left, abs_bottom, abs_back; // limitador, retirar

	SGWPosition first_region_p;
	V2 first_p;

	u8 gravity_type;
	SGWPosition world_gravity;
	V2 gravity_magnitude;

	SGWRegion *current_region;
	SGWRegionBlock *region_blocks;
	u32 region_block_count;

	SGWRegionGroupShapes *groups;
	u32 group_count;
};


internal b32 SGW_Validate (SGW *world);
internal void SGW_SetFlags (SGW *world, u32 flags);
internal SGWRegion *SGW_GetRegion(SGW *world, SGWPosition world_p);
internal SGWRegion *SGW_GetCurrentRegion(SGW *world, SGWPosition *world_p, V2 *relative_p, V2 size);
internal void SGW_UpdateAndRender (RendererState *renderer_state, System *system, SGW *world, b32 fullscreen, r32 dt);
inline V3 SGW_GetAbsolutePosition (SGWPosition world_p, V2 relative_p);
inline V3 SGW_GetPositionRelativeToView (SGW *world, V3 view);
inline void SGW_ApplyGravity (SGW *world, SGWPosition wp, V2 p, r32 mass, V2 *force, r32 dt);
#if DEBUG_BUILD

// struct SGWBuilderRegion
// {
// 	SGWRegion *region;
// 	SGWBuilderRegion *next;
// };
FORWARD_LINKED_LIST(SGWBuilderRegion,SGWRegion,region);

// struct SGWBuilderRegionGroupShapes
// {
// 	SGWRegionGroupShapes *group_shapes;
// 	SGWBuilderRegionGroupShapes *next;	
// };
FORWARD_LINKED_LIST(SGWBuilderRegionGroupShapes,SGWRegionGroupShapes,group_shapes);

#define SGWBUILDER_INIT_MEMORY MEGABYTES(64) 
struct SGWBuilder
{
	b32 initialized;
	MemoryPool memory;

	SGWBuilderRegion *first_region;
	SGWBuilderRegion *first_free_region;

	SGWBuilderRegionGroupShapes *first_group;
	SGWBuilderRegionGroupShapes *first_free_group;

	SGWPosition min, max;
	SGWPosition first_region_p;
	V2 first_p;
};

internal void SGW_BeginBuild (SGWBuilder *builder);
internal u32 SGW_RegionsFromSVG (const char *filename, SGWBuilder *builder, SGWPosition min, SGWPosition max);
internal SGW SGW_EndBuild (SGWBuilder *builder, MemoryPool *memory);
#endif