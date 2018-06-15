
#include <time.h>

internal void
SortRegionBlocks (SGW *world)
{
	for (u32 i = 0; i < world->region_block_count; ++i)
	{
		for (u32 j = i+1; j < world->region_block_count; ++j)
		{
			if(world->region_blocks[i].key > world->region_blocks[j].key)
			{
				SGWRegionBlock tmp = world->region_blocks[j];
				world->region_blocks[j] = world->region_blocks[i];
				world->region_blocks[i] = tmp;
			}
		}
	}
}

internal b32 
SGW_Validate (SGW *world)
{
	b32 valid = true;

	SortRegionBlocks(world);

	return valid;
}

internal SGW
debug_GenerateTestSGW(MemoryPool *memory)
{
	SGW res = {};

	res.first_region_p = {0,0,0};
	res.first_p = V2(0.0f,0.0f);

	srand(time(0));

	i32 right = 0;
	i32 left = 0;
	i32 top = 0;
	i32 bottom = 0;
	i32 front = 0;
	i32 back = 0;

	u32 rows = 3;
	u32 columns = 3;
	u32 layers = 1;

	b32 started = false;
	u32 block_count = rows*columns;
	SGWRegionBlock *blocks = AllocateFromMemoryPool(memory,SGWRegionBlock,block_count);
	SGWRegionBlock *block = blocks;
	for (i32 y = -((i32)rows/2); y <= ((i32)rows/2); ++y)
	{
		for (i32 x = -((i32)columns/2); x <= ((i32)columns/2); ++x)
		{
			u32 region_count = layers;
			SGWRegion *regions = AllocateFromMemoryPool(memory,SGWRegion,region_count);
			SGWRegion *region = regions;
			for (i32 z = 0; z < layers; ++z)
			{
				SGWPosition wp = {x,y,z};
				region->p = wp;

				if(started)
				{
					if(wp.x > right) right = wp.x;
					else if(wp.x < left) left = wp.x;
					if(wp.y > top) top = wp.y;
					else if(wp.y < bottom) bottom = wp.y;
					if(wp.z > front) front = wp.z;
					else if(wp.z < back) back = wp.z;
				}
				else
				{
					right = wp.x;
					left = wp.x;
					top = wp.y;
					bottom = wp.y;
					front = wp.z;
					back = wp.z;
					started = true;
				}

				region++;
			}

			block->key = SGWREGIONBLOCK_KEY(regions->p);
			block->regions = regions;
			block->region_count = region_count;
			
			++block;
		}
	}
	res.region_block_count = block_count;
	res.region_blocks = blocks;

	res.width = (right - left)+1;
	res.height = (top - bottom)+1;
	res.depth = (front - back)+1;

	res.abs_left   = (-((r32)res.width*0.5f)  * SGWREGION_WIDTH)  - (SGWREGION_WIDTH*0.5f);
	res.abs_bottom = (-((r32)res.height*0.5f) * SGWREGION_HEIGHT) - (SGWREGION_HEIGHT*0.5f);
	res.abs_back   = (-((r32)res.depth*0.5f)  * SGWREGION_DEPTH)  - (SGWREGION_DEPTH*0.5f);

	assert(SGW_Validate(&res));

	res.loaded = true;

	return res;
}

internal b32
Equals (SGWPosition a, SGWPosition b)
{
	b32 res = ((a.x==b.x) && (a.y==b.y) && (a.z==b.z));
	return res;
}

internal SGWRegion *
SGW_GetRegion(SGW *world, SGWPosition p)
{
	// TODO: tree?
	SGWRegion *res = 0;
	
	for (u32 i = 0; i < world->region_block_count; ++i)
	{
		SGWRegionBlock *block = world->region_blocks + i;
		for (u32 j = 0; j < block->region_count; ++j)
		{
			SGWRegion *region = block->regions + j;
			if(Equals(p, region->p))
			{
				res = region;
				break;
			}
		}
		if(res) break;
	}

	return res;	
}

internal SGWRegion *
SGW_GetCurrentRegion(SGW *world, SGWPosition *world_p, V2 *relative_p, V2 size)
{
	SGWRegion *res;

	V2 hsize = size * 0.5f;
	b32 left = (relative_p->x < SGWREGION_LEFT);
	b32 right = (relative_p->x > SGWREGION_RIGHT);
	b32 bottom = (relative_p->y < SGWREGION_BOTTOM);
	b32 top = (relative_p->y > SGWREGION_TOP);
	r32 x_correction = 0.0f;
	r32 y_correction = 0.0f;
	b32 clamp = false;

	if(left)
	{
		if(world_p->x > I32_MIN)
		{
			world_p->x -= 1;
			x_correction = +SGWREGION_WIDTH;
		}
		else clamp = true;
	}
	else if(right)
	{
		if(world_p->x < I32_MAX)
		{
			world_p->x += 1;
			x_correction = -SGWREGION_WIDTH;
		} 
		else clamp = true;
	}
	if(bottom)
	{
		if(world_p->y > I32_MIN)
		{
			world_p->y -= 1;
			y_correction = +SGWREGION_HEIGHT;
		}
		else clamp = true;
	}
	else if(top)
	{
		if(world_p->y < I32_MAX)
		{
			world_p->y += 1;
			y_correction = -SGWREGION_HEIGHT;
		}
		else clamp = true;
	}

	res = SGW_GetRegion(world, *world_p);
	if(!res) 
	{
		res = world->current_region;
		*world_p = res->p;
		clamp = true;
	}
	world->current_region = res;

	if(clamp)
	{
		relative_p->x = Clamp(SGWREGION_LEFT+hsize.x, SGWREGION_RIGHT-hsize.x, relative_p->x);
		relative_p->y = Clamp(SGWREGION_BOTTOM+hsize.y, SGWREGION_TOP-hsize.y, relative_p->y);
	}
	else
	{
		relative_p->x += x_correction;
		relative_p->y += y_correction;
	}

	return res;
}

inline V3
SGW_GetAbsolutePosition (SGWPosition world_p, V2 relative_p)
{
	V3 res = V3((world_p.x*SGWREGION_WIDTH) + relative_p.x,
				(world_p.y*SGWREGION_HEIGHT) + relative_p.y,
				(world_p.z*SGWREGION_DEPTH));
	return res;
}

inline V3 
SGW_GetPositionRelativeToView (SGW *world, V3 view)
{
	V3 res = {};
	return res;
}

inline void
SGW_ApplyGravity (SGW *world, SGWPosition wp, V2 p, r32 mass, V2 *force, r32 dt)
{
	if(world->gravity_type == SGW_GRAVITYTYPE_WORLD)
	{
		r32 magnitude = Magnitude(world->gravity_magnitude);
		V2 gravity_force;
		if(Equals(world->world_gravity, wp))
		{
			V2 p_to_center = V2(0.0f,0.0f) - p;
			Normalize(&p_to_center);
			gravity_force = p_to_center * (magnitude * mass * dt);
		}
		else
		{
			SGWPosition p_to_w = {(world->world_gravity.x - wp.x),(world->world_gravity.y - wp.y),0};
			V2 p_to_wrel = V2(p_to_w.x, p_to_w.y) - (V2(0.0f,0.0f) - p);
			Normalize(&p_to_wrel);
			gravity_force = p_to_wrel * (magnitude * mass * dt);
		}
		*force = *force + gravity_force;
	}
}

internal void
SGW_SetFlags(SGW *world, u32 flags)
{
	for (u32 i = 0; i < world->group_count; ++i)
	{
		SGWRegionGroupShapes *group = world->groups+i;
		group->image.flags = flags;
	}
}

internal void 
SGW_UpdateAndRender (RendererState *renderer_state, System *system, SGW *world, b32 fullscreen, r32 dt)
{

	Mat4 mvp = {};
	Mat4 model_matrix = {};

	SGWRegionGroupShapes *group = world->current_region->group;
	V3 p = SGW_GetAbsolutePosition(group->min->p, V2(-(SGWREGION_WIDTH*0.5f),-(SGWREGION_HEIGHT*0.5f)));
	p = p + V3(group->width*0.5f, group->height*0.5f, 0.0f);
	V2 s = V2(group->width,group->height);
	RenderVECImage(renderer_state, p, s, QUAT_ZERO, 1.0f, &group->image, fullscreen);

	ApplyTransforms(&model_matrix, V3(0.0f,0.0f,0.0f), V3(1.0f,1.0f,1.0f)*renderer_state->distance, QUAT_ZERO);
	mvp = (model_matrix * renderer_state->world_view_matrix) * renderer_state->world_projection_matrix;
	// CollisionPath *collision_path = group->paths;
	// while(collision_path)
	// {
	// 	for (u32 i = 0; i < collision_path->point_count-1; i+=3)
	// 	{
	// 		V3 p[] = 
	// 		{
	// 			V2_V3(collision_path->points[i+0], 0.01f),
	// 			V2_V3(collision_path->points[i+1], 0.01f),
	// 			V2_V3(collision_path->points[i+2], 0.01f),
	// 			V2_V3(collision_path->points[i+3], 0.01f)
	// 		};
	// 		RenderBezierImmediate(renderer_state, mvp, 1.0f, p[0], p[1], p[2], p[3], V3(1.0f,1.0f,1.0f));
	// 		// V3 colors[4];
	// 		// SET_COLORS(colors,V3(1.0f,0.3f,0.3f),4);
	// 		// RenderSolidImmediate (renderer_state, mvp, 1.0f, p, colors, 4, 0, 0, POINTS_);
	// 		// RenderSolidImmediate (renderer_state, mvp, 1.0f, p, colors, 4, 0, 0, LINE_STRIP);
	// 	}
	// 	collision_path = collision_path->next;
	// }

	for (u32 i = 0; i < world->region_block_count; ++i)
	{
		SGWRegionBlock *block = world->region_blocks + i;
		for (u32 j = 0; j < block->region_count; ++j)
		{
			SGWRegion *region = block->regions + j;
			SGWPosition wp = region->p;
			
			V3 p = SGW_GetAbsolutePosition(wp, V2(0.0f,0.0f));

			b32 is_current = (region == world->current_region);

			ApplyTransforms (&model_matrix, p*renderer_state->distance, V3(SGWREGION_WIDTH,SGWREGION_HEIGHT,1.0f)*0.99f*renderer_state->distance, QUAT_ZERO);
			mvp = (model_matrix * renderer_state->world_view_matrix) * renderer_state->world_projection_matrix;
			{
				V3 positions[] = {V3(-0.5f,0.5f,0.01f), V3(0.5f,0.5f,0.01f), V3(0.5f,-0.5f,0.01f), V3(-0.5f,-0.5f,0.01f)};
				V3 colors[4];
				SET_COLORS(colors,(is_current ? V3(0.5f,0.5f,0.5f):V3(0.2f,0.2f,0.2f)),4);
				RenderSolidImmediate (renderer_state, mvp, 1.0f, positions, colors, 4, 0, 0, LINE_LOOP);
			}
		}
	}
}

#if DEBUG_BUILD

internal void
SGW_BeginBuild (SGWBuilder *builder)
{
	if(!builder->memory.base || !builder->memory.size)
		InitializeMemoryPool (&builder->memory, SGWBUILDER_INIT_MEMORY);
	builder->initialized = true;
}

internal SGWBuilderRegionGroupShapes *
SGW_AddBuildGroup (SGWBuilder *builder, SGWRegionGroupShapes *group)
{
	assert(builder->initialized);
	SGWBuilderRegionGroupShapes *res = 0;

	if(builder->first_free_group)
	{
		res = builder->first_free_group;
		builder->first_free_group = builder->first_free_group->next;
	}
	else
	{
		res = AllocateFromMemoryPool(&builder->memory,SGWBuilderRegionGroupShapes,1);
	}

	// if(!builder->first_group)
	// {
	// 	builder->min = region->p;
	// 	builder->max = region->p;
	// }
	// else
	// {
	// 	if(builder->min.x > region->p.x) builder->min.x = region->p.x;
	// 	else if(builder->max.x < region->p.x) builder->max.x = region->p.x;
	// 	if(builder->min.y > region->p.y) builder->min.y = region->p.y;
	// 	else if(builder->max.y < region->p.y) builder->max.y = region->p.y;
	// }

	res->group_shapes = group;
	res->next = builder->first_group;
	builder->first_group = res;

	return res;
}

internal SGWBuilderRegion *
SGW_AddBuildRegion (SGWBuilder *builder, SGWRegion *region)
{
	assert(builder->initialized);
	SGWBuilderRegion *res = 0;

	if(builder->first_free_region)
	{
		res = builder->first_free_region;
		builder->first_free_region = builder->first_free_region->next;
	}
	else
	{
		res = AllocateFromMemoryPool(&builder->memory,SGWBuilderRegion,1);
	}

	if(!builder->first_region)
	{
		builder->min = region->p;
		builder->max = region->p;
	}
	else
	{
		if(builder->min.x > region->p.x) builder->min.x = region->p.x;
		else if(builder->max.x < region->p.x) builder->max.x = region->p.x;
		if(builder->min.y > region->p.y) builder->min.y = region->p.y;
		else if(builder->max.y < region->p.y) builder->max.y = region->p.y;
	}

	res->region = region;
	res->next = builder->first_region;
	builder->first_region = res;

	return res;
}

internal void 
SGW_RemoveBuildRegion (SGWBuilder *builder, SGWBuilderRegion *bregion)
{
	assert(builder->initialized);
}

inline void
AddCurve(V2 p0, V2 p1, V2 p2, V2 p3, V2 **points, u32 *count, MemoryPool *memory)
{
	V2 *next = AllocateFromMemoryPool(memory, V2, 4);
	if(!*points) *points = next;
	next[0] = p0;
	next[1] = p1;
	next[2] = p2;
	next[3] = p3;
	*count += 4;
}

internal CollisionPath *
SGW_GetCollisionPathsInRegion (SGWPosition region_p, CollisionPath *group_collision_paths, MemoryPool *memory)
{
	CollisionPath *res = 0;

	V2 min = SGW_GetAbsolutePosition(region_p, V2(SGWREGION_WIDTH*-0.5f,SGWREGION_HEIGHT*-0.5f)).xy;
	V2 max = min + V2(SGWREGION_WIDTH,SGWREGION_HEIGHT);

	V2 center = SGW_GetAbsolutePosition(region_p, V2(0.0f,0.0f)).xy;
	V2 size = V2(SGWREGION_WIDTH, SGWREGION_HEIGHT);

	V2 bl_tl[] = {min, (min+V2(0.0f,SGWREGION_HEIGHT))};
	V2 tl_tr[] = {bl_tl[1], (bl_tl[1]+V2(SGWREGION_WIDTH,0.0f))};
	V2 tr_br[] = {tl_tr[1], (tl_tr[1]-V2(0.0f,SGWREGION_HEIGHT))};
	V2 br_bl[] = {tr_br[1], (tr_br[1]-V2(SGWREGION_WIDTH,0.0f))};

	V2 *bounds[] = {bl_tl, tl_tr, tr_br, br_bl};

	CollisionPath *collision_paths = AllocateFromMemoryPool(memory, CollisionPath, 1);
	CollisionPath *collision_path = collision_paths;
	CollisionPath *group_collision_path = group_collision_paths;
	while(group_collision_path)
	{
		collision_path->point_count = 0;
		V2 *next = 0;
		for (u32 i = 0; i < group_collision_path->point_count-1; i+=3)
		{
			V2 p0 = group_collision_path->points[i+0];
			V2 p1 = group_collision_path->points[i+1];
			V2 p2 = group_collision_path->points[i+2];
			V2 p3 = group_collision_path->points[i+3];

			b32 p0in = IsPointInsideRect(p0, center, size);
			b32 p1in = IsPointInsideRect(p1, center, size);
			b32 p2in = IsPointInsideRect(p2, center, size);
			b32 p3in = IsPointInsideRect(p3, center, size);

			if(p0in && p1in && p2in && p3in)
			{ // curva completamente dentro da region
				AddCurve(p0, p1, p2, p3, &collision_path->points, &collision_path->point_count, memory);
			}
			else if(p0in || p1in || p2in || p3in)
			{ // pode estar dividida entre regions
				//TODO: Dividir Curvas
				
				AddCurve(p0, p1, p2, p3, &collision_path->points, &collision_path->point_count, memory);

				// b32 partly_in = false;
				// b32 vertical = true;
				// r32 t[3];
				// for (u32 i = 0; i < 4; ++i)
				// {

				// 	vertical = !vertical;
				// }
				// partly_in = true;

				// if(!partly_in && (p0in && p3in)) // pontos de controle fora, curva dentro
				// 	AddCurve(p0, p1, p2, p3, &collision_path->points, &collision_path->point_count, memory);
			}
		}

		group_collision_path = group_collision_path->next;
	}

	return res;
}

internal u32
SGW_RegionsFromSVG (const char *filename, SGWBuilder *builder, SGWPosition min, SGWPosition max)
{
	assert(builder->initialized);
	assert((min.x <= max.x) && (min.y <= max.y) && (min.z == max.z));

	u32 count = 0;

	NSVGimage *image = nsvgParseFromFile(filename, "px", 96);
	r32 image_width = image->width;
	r32 image_height = image->height;
	r32 image_ratio = image_width/image_height;
	// u32 shape_count = 0;
	// for (NSVGshape *shape = image->shapes; shape != 0; shape = shape->next) shape_count++;

	u32 x_span = (max.x - min.x) + 1;
	r32 width = x_span * SGWREGION_WIDTH;

	u32 y_span = (max.y - min.y) + 1;
	r32 height = y_span * SGWREGION_HEIGHT;
	
	// r32 region_norm_width = image_ratio/x_span;
	// r32 region_norm_height = 1.0f/y_span;
	// V2 rsize = V2(region_norm_width, region_norm_height);
	// r32 norm_x = 0.0f, norm_y = 0.0f;
	V3 center = SGW_GetAbsolutePosition(min, V2((SGWREGION_WIDTH*-0.5f),(SGWREGION_HEIGHT*-0.5f)));
	center = center + V3(width*0.5f, height*0.5f, 0.0f);

	u32 w = image->width;
	u32 h = image->height;
	CollisionPath *collision_paths = 0;
	for(NSVGshape *shape = image->shapes; shape != NULL; shape = shape->next)
	{
		if((shape->next == NULL) && (shape->paths))
		{
			collision_paths = AllocateFromMemoryPool(&builder->memory, CollisionPath, 1);
			CollisionPath *collision_path = collision_paths;
			for(NSVGpath *path = shape->paths; path != NULL; path = path->next)
			{
				collision_path->point_count = path->npts;
				collision_path->points = AllocateFromMemoryPool(&builder->memory, V2, collision_path->point_count);
				for (u32 i = 0; i < path->npts; i++)
				{
					collision_path->points[i] = V2(((path->pts[(i*2)+0]/w)-0.5f)*width, 
												  ((-path->pts[(i*2)+1]/h)+0.5f)*height) + center.xy;

				}
				if(path->next != NULL) 
					collision_path->next = AllocateFromMemoryPool(&builder->memory, CollisionPath, 1);
				collision_path = collision_path->next;
			}
		}
	}

	SGWRegionGroupShapes *group = AllocateFromMemoryPool(&builder->memory,SGWRegionGroupShapes,1);
	group->image.nsvg_img = image;
	group->image.width = width;
	group->image.height = height;
	group->image.flags = VECIMAGE_FLAG_RASTERIZE;
	group->width = width;
	group->height = height;
	
	u32 region_count = (x_span*y_span);
	SGWBuilderRegion **bregions = AllocateFromMemoryPool(&builder->memory,SGWBuilderRegion*,region_count);
	SGWBuilderRegion **bregion = bregions;
	NSVGimage *region_images = AllocateFromMemoryPool(&builder->memory,NSVGimage,region_count);
	NSVGimage *region_image = region_images;
	for (i32 x = min.x; x <= max.x; ++x)
	{
		for (i32 y = min.y; y <= max.y; ++y)
		{
			SGWRegion *region = AllocateFromMemoryPool(&builder->memory,SGWRegion,1);
			region->p = {x, y, min.z};
			region->group = group;

			region->collision_paths = SGW_GetCollisionPathsInRegion(region->p, collision_paths, &builder->memory);

			*(bregion++) = SGW_AddBuildRegion(builder, region);
		}
	}

	group->min = bregions[0]->region;
	group->max = bregions[region_count-1]->region;
	SGW_AddBuildGroup(builder, group);

	// nsvgDelete(image);
	return count;
}

internal SGW 
SGW_EndBuild (SGWBuilder *builder, MemoryPool *memory)
{
	assert(builder->initialized);

	SGW res = {};

	u8 *begginning = PushMemoryPool(memory);
	b32 error = false;

	// contando RegionBlocks
	u32 region_block_count = 0;
	for (i32 x = builder->min.x; x <= builder->max.x; ++x)
	{
		for (i32 y = builder->min.y; y <= builder->max.y; ++y)
		{
			SGWBuilderRegion *bregion = builder->first_region;
			u32 region_count = 0;
			while(bregion)
			{
				SGWRegion *region = bregion->region;
				if((region->p.x == x)&&(region->p.y == y)) 
				{
					++region_count;
					break;
				}
				bregion = bregion->next;
			}
			if(region_count > 0) region_block_count++;
		}
	}
	
	// Copiando Regions p/ RegionBlocks
	SGWRegionBlock *region_blocks = AllocateFromMemoryPool(memory,SGWRegionBlock,region_block_count);
	region_block_count = 0;
	if(region_blocks)
	{
		for (i32 x = builder->min.x; x <= builder->max.x; ++x)
		{
			for (i32 y = builder->min.y; y <= builder->max.y; ++y)
			{
				SGWBuilderRegion *bregion = builder->first_region;
				u32 region_count = 0;
				while(bregion)
				{
					SGWRegion *region = bregion->region;
					if((region->p.x == x)&&(region->p.y == y)) 
					{
						region_count++;
					}
					bregion = bregion->next;
				}
				SGWRegion *regions = AllocateFromMemoryPool(memory,SGWRegion,region_count);
				region_count = 0;
				if(regions)
				{
					bregion = builder->first_region;
					while(bregion)
					{
						SGWRegion *region = bregion->region;
						if((region->p.x == x)&&(region->p.y == y)) 
						{
							regions[region_count] = *region;
							CollisionPath *b_collision_path = region->collision_paths;
							while(b_collision_path)
							{
								
								b_collision_path = b_collision_path->next;
							}
							region_count++;
						}
						bregion = bregion->next;
					}

					SGWRegionBlock block = {};
					block.key = SGWREGIONBLOCK_KEY(regions[0].p);
					block.regions = regions;
					block.region_count = region_count;
					region_blocks[region_block_count++] = block;
				}
			}
		}
	}

	// Checando overlaps
	for (u32 i = 0; i < region_block_count; ++i)
	{
		SGWRegionBlock *block = region_blocks + i;
		for (u32 j = 0; j < block->region_count; ++j)
		{
			for (u32 k = 0; k < block->region_count; ++k)
			{
				if(j != k)
				{
					if(block->regions[j].p.z == block->regions[k].p.z)
					{
						error = true;
						char text[128];
						sprintf(text, "Overlap em [%d,%d]!", block->regions[j].p.x,block->regions[j].p.y);
						ShowSystemPopupTextWindow ("Erro", text);
						break;
					}
					if(error) break;
				}
			}
			if(error) break;
		}
		if(error) break;
	}

	// Criando mundo
	if(!error)
	{
		res.width = (builder->max.x - builder->min.x)+1;
		res.height = (builder->max.y - builder->min.y)+1;
		res.depth = (builder->max.z - builder->min.z)+1;

		res.abs_left   = (-((r32)res.width*0.5f)  * SGWREGION_WIDTH)  - (SGWREGION_WIDTH*0.5f);
		res.abs_bottom = (-((r32)res.height*0.5f) * SGWREGION_HEIGHT) - (SGWREGION_HEIGHT*0.5f);
		res.abs_back   = (-((r32)res.depth*0.5f)  * SGWREGION_DEPTH)  - (SGWREGION_DEPTH*0.5f);

		res.first_region_p = builder->first_region_p;
		res.first_p = builder->first_p;

		res.gravity_type = 0;
		res.world_gravity = {0,0,0};
		res.gravity_magnitude = V2(0.0f,0.0f);

		res.region_blocks = region_blocks;
		res.region_block_count = region_block_count;

		// groups
		u32 group_count = 0;
		for(SGWBuilderRegionGroupShapes *bgroup = builder->first_group; bgroup != 0; bgroup = bgroup->next) group_count++;
		SGWRegionGroupShapes *groups = AllocateFromMemoryPool(memory, SGWRegionGroupShapes, group_count);
		SGWRegionGroupShapes *group = groups;
		for(SGWBuilderRegionGroupShapes *bgroup = builder->first_group; bgroup != 0; bgroup = bgroup->next)
		{
			*group = *bgroup->group_shapes;

			group->min = SGW_GetRegion(&res, group->min->p);
			group->max = SGW_GetRegion(&res, group->max->p);
			
			SGWPosition minp = group->min->p;
			SGWPosition maxp = group->max->p;
			for (i32 y = minp.y; y <= maxp.y; ++y)
			{
				for (i32 x = minp.x; x <= maxp.x; ++x)
				{
					SGWRegion *region = SGW_GetRegion(&res, {x,y,minp.z});
					region->group = group;
				}
			}

			group++;
		}
		res.groups = groups;
		res.group_count = group_count;

		res.loaded = true;

		FreeMemoryPool(&builder->memory);
	}

	return res;
}

#endif