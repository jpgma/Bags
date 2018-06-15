
#define ABS_FROM_REL_POINTER(type,origin,relative) ((type)*)(((u8*)(relative))+(origin))

internal b32
VectorImageToFile (const char *filename, VectorImage vector_image)
{
	b32 res = false;

	u32 ngroups = 0;
	u32 npaths = 0;
	u32 npoints = 0;
	{ // medindo vector_image
		VIPathGroup *g = vector_image.first_group;
		while(g)
		{
			VIPath *p = g->first_path;
			while(p)
			{
				npoints += p->npoints;
				p = p->next;
				++npaths;
			}
			g = g->next;
			++ngroups;
		}
	}

	if(ngroups && npaths && npoints)
	{ // vector_image n vazio
		u32 size = sizeof(VectorImage) + (ngroups*sizeof(VIPathGroup))
				   + (npaths*sizeof(VIPath)) + (npoints*sizeof(V2));
		u8 *data = (u8*)PlatformAllocateMemory(size);

		VectorImage *vi = (VectorImage*)data;
		*vi = vector_image;
		vi->first_group = (VIPathGroup*)sizeof(VectorImage);

		VIPathGroup *dst_g = (VIPathGroup*)(vi+1);
		VIPathGroup *src_g = vector_image.first_group;
		while(src_g)
		{
			*dst_g = *src_g;

			VIPath *dst_p = (VIPath*)(dst_g+1);
			VIPath *src_p = src_g->first_path;
			while(src_p)
			{
				*dst_p = *src_p;
				V2 *src_point = src_p->points;
				V2 *dst_point = src_p->points;
				
				src_p = src_p->next;
				dst_p += 1;
			}

			src_g = src_g->next;
			dst_g += 1;
		}

		res = PlaformWriteEntireFile(filename, data, size);
		PlatformDeallocateMemory(data,size);
	}


	return res;
}


internal VectorImage
VectorImageFromFile (const char *filename)
{
	VectorImage res = {};

	RawFileContents vif = PlatformGetRawFileContents(filename, 0);
	u8 *raw_vector_image = vif.contents;
	if(raw_vector_image)
	{

	}
	
	return res;
}

internal void 
RasterizeVectorImage (VectorImage vector_image, u32 width, u32 height, r32 scale, u8 *pixels)
{
	
}
