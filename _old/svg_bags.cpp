
internal char *
SVG_GetTagName (char *text, MemoryPool *memory)
{
	i32 lenght1 = GetLengthUntil(text+1, ' ');
	if(lenght1<0) lenght1 = GetLength(text+1);
	i32 lenght2 = GetLengthUntil(text+1, '>');
	if(lenght2<0) lenght2 = GetLength(text+1);
	u32 lenght = ((lenght1 < lenght2) ? lenght1 : lenght2)+1;
	
	char *tag_name = AllocateFromMemoryPool(memory, char, lenght);
	
	for (u32 i = 0; i < lenght-1; i++) tag_name[i] = *(text+1+i);
	tag_name[lenght-1] = 0;

	return tag_name;
}

internal b32
SVG_IsTagSelfEnclosed (char *text)
{
	if(*(text+1)=='/') return true;
	
	char *c = text;
	while((*(c)!='>')&&(*(c)!='\0')) c++;
	if(*(c-1)=='/') return true;

	return false;
}

internal b32
SVG_IsClosureTag(char *text)
{
	if((*(text+1)=='/')) return true;
	else return false;
}

internal char *
SVG_GetAttrib (char *text, char *attrib_name, MemoryPool *memory)
{
	char *res = 0;

	char *c = text;
	char *d = Append(attrib_name, "=\"");
	u32 d_length = GetLength(d);
	char *attrib_text = FindSubstring(text, d);
	if(attrib_text)
	{
		attrib_text += d_length;
		u32 text_length = GetLengthUntil(attrib_text, '\"')+2;

		res = AllocateFromMemoryPool(memory, char, text_length);
		for (int i = 0; i < text_length-1; i++) res[i] = attrib_text[i];
		res[text_length-1] = 0;
	}
	return res;
}

inline u32
SVG_GetNextU32 (char *text, char **after_text)
{
	u32 res = 0;
	
	while((*text < '0') || (*text > '9')) text++;
	u32 length = 0;
	while((*text >= '0') && (*text <= '9')) 
	{
		length++;
		text++;
	}
	char *n_text = text - length;
	res = (u32)atoi(n_text);

	if(after_text) *after_text = text;
	return res;
}

inline r32
SVG_GetNextR32 (char *text, char **after_text)
{
	r32 res = 0;
	
	while((*text < '0') || (*text > '9')) text++;
	b32 point = (*(text-1)=='.');
	b32 negative = (*(text-(point ? 2 : 1))=='-');
	u32 length = 1 + (negative ? 1 : 0);
	while((*text >= '0') && (*text <= '9')) 
	{
		length++;
		text++;
	}
	if(*text == '.')
	{
		do
		{
			length++;
			text++;
		}	
		while((*text >= '0') && (*text <= '9'));
	}
	length -= 1;
	char *n_text = text - length;
	res = (r32)atof(n_text);

	if(after_text) *after_text = text;
	return res;
}

inline char *
SVG_GetNextTag (char *text)
{
	char *res = 0;
	i32 offset = GetLengthUntil(text, '<');
	if(offset>=0) res = text+offset;
	return res;
}

inline V2
SVG_GetNextXYPair (char *text, char **after_text)
{
	V2 res = V2(SVG_GetNextR32(text, after_text),
				SVG_GetNextR32(*after_text, after_text));
	return res;
}

// internal VECImage
// SVG_GetVECImage(char *svg_text, r32 z_diff, MemoryPool *memory)
// {
// 	VECImage res = {};

// 	NSVGimage *image = nsvgParse(svg_text, "px", 96);
// 	// u32 shape_count = 0;
// 	// for (NSVGshape *shape = image->shapes; shape != 0; shape = shape->next) shape_count++;
// 	res.width = image->width;
// 	res.height = image->height;
// 	res.shapes = AllocateFromMemoryPool(memory, VECShape, 1);

// 	u32 height = 5.0f;
// 	u32 width = (res.width/res.height)*height;

// 	r32 z = 0.0f;
// 	VECShape *vecshape = res.shapes;
// 	for (NSVGshape *shape = image->shapes; shape != 0; shape = shape->next)
// 	{
// 		vecshape->z = z;
// 		z += z_diff;
// 		vecshape->bounds = V4(((-shape->bounds[0]/res.width)+0.5f)*width, ((-shape->bounds[1]/res.height)+0.5f)*height,
// 							  ((-shape->bounds[2]/res.width)+0.5f)*width, ((-shape->bounds[3]/res.height)+0.5f)*height);
// 		vecshape->paths = AllocateFromMemoryPool(memory, VECPath, 1);
// 		{
// 			i32 i, cr, cg, cb, ca;
// 			cr = shape->fill.color & 0xff;
// 			cg = (shape->fill.color >> 8) & 0xff;
// 			cb = (shape->fill.color >> 16) & 0xff;
// 			ca = (shape->fill.color >> 24) & 0xff;

// 			i32 r,g,b;
// 			i32 a = ca/255;
// 			i32 ia = 255 - a;
// 			// Premultiply
// 			r = (cr * a)/255;
// 			g = (cg * a)/255;
// 			b = (cb * a)/255;

// 			// Blend over
// 			r += (ia * 1)/255;
// 			g += (ia * 1)/255;
// 			b += (ia * 1)/255;
// 			a += (ia * 1)/255;

// 			vecshape->fill = V3((((u32)r)/255.0f),(((u32)g)/255.0f),(((u32)b)/255.0f));
// 		}

// 		VECPath *vecpath = vecshape->paths;
// 		for (NSVGpath *path = shape->paths; path != 0; path = path->next)
// 		{
// 			vecpath->bounds = V4(((-path->bounds[0]/res.width)+0.5f)*width, ((-path->bounds[1]/res.height)+0.5f)*height,
// 								 ((-path->bounds[2]/res.width)+0.5f)*width, ((-path->bounds[3]/res.height)+0.5f)*height);
// 			vecpath->cbezier_point_count = path->npts;
// 			vecpath->cbezier_points = AllocateFromMemoryPool(memory, V2, vecpath->cbezier_point_count);
// 			V2 *point = vecpath->cbezier_points;
// 			for (u32 i = 0; i < path->npts; i++)
// 			{
// 				r32 *p = &path->pts[i*2];
// 				*(point++) = V2(((-p[0]/res.width)+0.5f)*width, ((-p[1]/res.height)+0.5f)*height);
// 			}
			
// 			if(path->next) vecpath->next = AllocateFromMemoryPool(memory, VECPath, 1);
// 			vecpath = vecpath->next;
// 		}

// 		if(shape->next) vecshape->next = AllocateFromMemoryPool(memory, VECShape, 1);
// 		vecshape = vecshape->next;
// 	}

// 	return res;
// }