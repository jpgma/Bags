
#if 0

internal void
PackSGW (AssetManagerState *asset_state, SGW world, MemoryPool *memory)
{
	if(asset_state->initialized && SGW_Validate(&world))
	{
		u8 *beginning = PushMemoryPool(memory);
		BagHeader *header = AllocateFromMemoryPool(memory,BagHeader,1);
		header->tag = TAG_SGW;
		header->count = 1;
		header->beginning = (u64)beginning;
		SGW *world_ = AllocateFromMemoryPool(memory,SGW,1);
		*world_ = world;
		
		// region blocks
		SGWRegionBlock *blocks = AllocateFromMemoryPool(memory,SGWRegionBlock,world.region_block_count);
		for (u32 i = 0; i < world.region_block_count; ++i)
		{
			blocks[i] = world.region_blocks[i];
		}
		world_->region_blocks = blocks;
		for (u32 i = 0; i < world.region_block_count; ++i)
		{
			SGWRegionBlock *block = world.region_blocks+i;
			SGWRegion *regions = AllocateFromMemoryPool(memory,SGWRegion,block->region_count);
			for (u32 j = 0; j < block->region_count; ++j)
			{
				regions[j] = block->regions[j];
			}
			world_->region_blocks[i].regions = regions;
		}

		// groups
		u32 group_count = world.group_count;
		SGWRegionGroupShapes *groups = AllocateFromMemoryPool(memory, SGWRegionGroupShapes, group_count);
		VECImage *images = AllocateFromMemoryPool(memory, VECImage, group_count);
		for (u32 i = 0; i < group_count; ++i)
		{
			// SGWRegionGroupShapes *group = groups + i;
			// *group = world.groups[i];

			// images[i] = *(group->image.nsvg_img);
			// group->image = images+i;
			// u32 shape_count = 0;
			// for (NSVGshape *shape = group->image->shapes; shape != 0; shape = shape->next) shape_count++;
			// NSVGshape *image_shapes = AllocateFromMemoryPool(memory, NSVGshape, shape_count);
			// NSVGshape *image_shape = image_shapes;
			// for (NSVGshape *shape = group->image->shapes; shape != 0; shape = shape->next)
			// {
			// 	*image_shape = *shape;
			// 	u32 path_count = 0;
			// 	for (NSVGpath *path = image_shape->paths; path != 0; path = path->next) path_count++;
			// 	NSVGpath *shape_paths = AllocateFromMemoryPool(memory, NSVGpath, path_count);
			// 	NSVGpath *shape_path = shape_paths;
			// 	for (NSVGpath *path = image_shape->paths; path != 0; path = path->next)
			// 	{
			// 		*shape_path = *path;
			// 		u32 point_count = (path->npts*2);
			// 		shape_path->pts = AllocateFromMemoryPool(memory,r32,point_count);
			// 		for (u32 j = 0; j < point_count; ++j) shape_path->pts[j] = path->pts[j];
					
			// 		if(path->next)
			// 		{
			// 			shape_path->next = shape_path+1;
			// 			shape_path++;
			// 		}
			// 	}
			// 	image_shape->paths = shape_paths;

			// 	if(shape->next)
			// 	{
			// 		image_shape->next = image_shape+1;
			// 		image_shape++;
			// 	}
			// }
			// group->image->shapes = image_shapes;

			// Shape *internal_shapes = AllocateFromMemoryPool(memory, Shape, group->shape_count);
			// Shape *internal_shape = internal_shapes;
			// for (u32 j = 0; j < group->shape_count; ++j)
			// {
			// 	*internal_shape = group->shapes[j];
			// 	internal_shape->shape = image_shapes+j;
			// 	Path *internal_paths = AllocateFromMemoryPool(memory, Path, internal_shape->path_count);
			// 	Path *internal_path = internal_paths;
			// 	for (u32 k = 0; k < internal_shape->path_count; ++k)
			// 	{
			// 		*internal_path = internal_shape->paths[k];
			// 		V2 *internal_points = AllocateFromMemoryPool(memory, V2, internal_path->point_count);
			// 		for (u32 l = 0; l < internal_path->point_count; ++l) internal_points[l] = internal_path->points[l];
			// 		internal_path->points = internal_points;
			// 	}
			// 	internal_shape->paths = internal_paths;
			// 	internal_shape++;
			// }
			// group->shapes = internal_shapes;

			// group->min = SGW_GetRegion(world_, group->min->p);
			// group->max = SGW_GetRegion(world_, group->max->p);
			
			// SGWPosition minp = group->min->p;
			// SGWPosition maxp = group->max->p;
			// for (i32 y = minp.y; y <= maxp.y; ++y)
			// {
			// 	for (i32 x = minp.x; x <= maxp.x; ++x)
			// 	{
			// 		SGWRegion *region = SGW_GetRegion(world_, {x,y,minp.z});
			// 		region->group = group;
			// 	}
			// }
		}
		// world_->groups = groups;

		// relative pointers
		// for (u32 i = 0; i < world.region_block_count; ++i) // TODO: fazer funcoes p/ simplificar essas serializaçoes
		// {
		// 	SGWRegionBlock *block = world_->region_blocks+i;
		// 	for (u32 j = 0; j < block->region_count; ++j)
		// 	{
		// 		i64 offset = ((i64)block->regions[j].group) - ((i64)begginning);
		// 		block->regions[j].group = (SGWRegionGroupShapes*)offset;
		// 	}
		// 	i64 offset = ((i64)block->regions) - ((i64)begginning);
		// 	block->regions = (SGWRegion*)offset;
		// }
		// i64 offset = ((i64)world_->region_blocks) - ((i64)begginning);
		// world_->region_blocks = (SGWRegionBlock*)offset;
		// for (u32 i = 0; i < world.group_count; ++i)
		// {
		// 	SGWRegionGroupShapes *group = world_->groups+i;
		// 	for (u32 j = 0; j < group->shape_count; ++j)
		// 	{

		// 	}
		// 	i64 offset = ((i64)group->image) - ((i64)begginning);
		// 	group->image = (NSVGimage*)offset;
		// 	offset = ((i64)group->min) - ((i64)begginning);
		// 	group->min = (SGWRegion*)offset;
		// 	offset = ((i64)group->max) - ((i64)begginning);
		// 	group->max = (SGWRegion*)offset;
		// }
		// offset = ((i64)world_->groups) - ((i64)begginning);
		// world_->groups = (SGWRegionGroupShapes*)offset;

		// u32 size = PushMemoryPool(memory) - beginning;
		// WriteFile(asset_state->world_bag_path, size, (void *)beginning);
	}
}

internal void 
UpdateAndRenderAssetManagerPanel (AssetManagerState *asset_state)
{

}


internal void
InitAssetManager (AssetManagerState *asset_state)
{
	const char world_bag_path[] = "data\\world.bag";
	CopyString(world_bag_path,asset_state->world_bag_path,(sizeof(world_bag_path)));
	const char fonts_bag_path[] = "data\\fonts.bag";
	CopyString(fonts_bag_path,asset_state->fonts_bag_path,(sizeof(fonts_bag_path)));
	asset_state->initialized = true;
}

internal SGW *
LoadEntireSGW (AssetManagerState *asset_state, MemoryPool *memory)
{
	SGW *res = 0;
	RawFileContents file = GetRawFileContents(asset_state->world_bag_path, memory);
	u8 *data = (u8 *)file.contents;
	if(file.contents && (U32Swap(U32_FROM_U8P(data)) == TAG_SGW))
	{
		BagHeader *header = (BagHeader*)data;
		res = (SGW*)(data+sizeof(BagHeader));

		res->region_blocks = BAG_POINTER(header,data,SGWRegionBlock,res->region_blocks);//(SGWRegionBlock*)(data + (((i64)res->region_blocks)-beggining));
		for (u32 i = 0; i < res->region_block_count; ++i)
		{
			SGWRegionBlock *block = res->region_blocks+i;
			block->regions = BAG_POINTER(header,data,SGWRegion,block->regions);//(SGWRegion*)(data+((i64)block->regions));
			for (u32 j = 0; j < block->region_count; ++j)
			{
				SGWRegion *region = block->regions+j;
				region->group = BAG_POINTER(header,data,SGWRegionGroupShapes,region->group);//(SGWRegionGroupShapes*) (data+((i64)region->group));
			}
		}

		res->groups = BAG_POINTER(header,data,SGWRegionGroupShapes,res->groups);//(SGWRegionGroupShapes*) (data+((i64)res->groups));
		for (u32 i = 0; i < res->group_count; ++i)
		{
			SGWRegionGroupShapes *group = res->groups+i;
			group->image = BAG_POINTER(header,data,NSVGimage,group->image);//(NSVGimage*)(data+((i64)group->image));
			group->image->shapes = BAG_POINTER(header,data,NSVGshape,group->image->shapes);
			for (NSVGshape *shape = group->image->shapes; shape != 0; shape = shape->next)
			{
				shape->paths = BAG_POINTER(header,data,NSVGpath,shape->paths);
				for (NSVGpath *path = shape->paths; path != 0; path = path->next)
				{
					path->pts = BAG_POINTER(header,data,r32,path->pts);
					path->next = BAG_POINTER(header,data,NSVGpath,path->next);
				}
				shape->next = BAG_POINTER(header,data,NSVGshape,shape->next);
			}
			// group->shapes = BAG_POINTER(header,data,Shape,group->shapes);
			// for (u32 j = 0; j < group->shape_count; ++j)
			// {
			// 	Shape *shape = group->shapes+j;
			// 	shape->shape = BAG_POINTER(header,data,NSVGshape,shape->shape);
			// 	shape->paths = BAG_POINTER(header,data,Path,shape->paths);
			// 	for (u32 k = 0; k < shape->path_count; ++k)
			// 	{
			// 		Path *path = shape->paths+k;
			// 		path->points = BAG_POINTER(header,data,V2,path->points);
			// 	}
			// }
			group->min = BAG_POINTER(header,data,SGWRegion,group->min);
			group->max = BAG_POINTER(header,data,SGWRegion,group->max);
		}

		res->loaded = true;
	}
	return res;
}
#endif