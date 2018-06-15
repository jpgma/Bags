/*
   Char. number range  |        UTF-8 octet sequence
      (hexadecimal)    |              (binary)
   --------------------+---------------------------------------------
   0000 0000-0000 007F | 0xxxxxxx
   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/

internal FontBAG
GetFontBAG (const char *filename, MemoryPool *memory, MemoryPool *temp_memory)
{
	FontBAG res = {};

	u8 *pop = PushMemoryPool(temp_memory);
	RawFileContents fonts_file = GetRawFileContents(filename, temp_memory);
	u8 *data = (u8 *)fonts_file.contents;
	if(fonts_file.contents && (U32Swap(U32_FROM_U8P(data)) == TAG_FONT))
	{
		BagHeader header = *((BagHeader*)data);
		u32 font_count = header.count;
		data+=sizeof(BagHeader);

		res.font_count = font_count;
		res.fonts = AllocateFromMemoryPool(memory, Font, font_count);

		for (u32 i = 0; i < font_count; ++i)
		{
			Font *font = &res.fonts[i];

			u32 font_size = U32Swap(U32_FROM_U8P(data));
			data += sizeof(u32);
			u32 name_length = U32Swap(U32_FROM_U8P(data));
			data += sizeof(u32);
			char *font_name = AllocateFromMemoryPool(memory,char,name_length+1);
			for (u32 j = 0; j < name_length; ++j)
				font_name[j] = *((char*)data+j);
			font_name[name_length] = 0;
			font->name = font_name;
			data += name_length;
			

			font->ascent = *((i32*)data);
			data += sizeof(i32);
			
			font->descent = *((i32*)data);
			data += sizeof(i32);
			
			font->line_gap = *((i32*)data);
			data += sizeof(i32);

			font->font_height = *((i32*)data);
			data += sizeof(i32);
			
			font->units_per_em = *((u32*)data);
			data += sizeof(u32);
			
			font->glyph_count = U32Swap(U32_FROM_U8P(data));
			data += sizeof(u32);
			font->glyphs = AllocateFromMemoryPool(memory,Glyph,font->glyph_count);

			b32 started = false;
			for (u32 i = 0; i < font->glyph_count; ++i)
			{
				font->glyphs[i].unicode_codepoint = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);

				font->glyphs[i].vertex_count = U32Swap(U32_FROM_U8P(data));
				data += sizeof(u32);

				font->glyphs[i].advance_width = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);

				font->glyphs[i].left_bearing = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);

				font->glyphs[i].min_x = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);
				if(!started || (font->glyphs[i].min_x < font->min_x)) 
					font->min_x = font->glyphs[i].min_x;
				
				font->glyphs[i].min_y = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);
				if(!started || (font->glyphs[i].min_y < font->min_y)) 
					font->min_y = font->glyphs[i].min_y;
				
				font->glyphs[i].max_x = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);
				if(!started || (font->glyphs[i].max_x > font->max_x)) font->max_x = font->glyphs[i].max_x;
				
				font->glyphs[i].max_y = I32Swap(I32_FROM_U8P(data));
				data += sizeof(i32);
				if(!started || (font->glyphs[i].max_y > font->max_y)) 
					font->max_y = font->glyphs[i].max_y;

				if(!started) started = true;

				font->glyphs[i].bitmap = 0;
				font->glyphs[i].size = 0;
				font->glyphs[i].w = 0;
				font->glyphs[i].h = 0;
				font->glyphs[i].id = 0;

				font->glyphs[i].vertices = AllocateFromMemoryPool(memory,FontVertex,font->glyphs[i].vertex_count);
				for (u32 j = 0; j < font->glyphs[i].vertex_count; ++j)
				{
					font->glyphs[i].vertices[j] = *((FontVertex*)data);
					data += sizeof(FontVertex);
				}
			}

			font->kern_count = U32Swap(U32_FROM_U8P(data));
			data += sizeof(u32);
			font->kern = AllocateFromMemoryPool(memory,KernEntry,font->kern_count);
			for (u32 i = 0; i < font->kern_count; ++i)
			{
				font->kern[i] = *((KernEntry*)data);
				data += sizeof(KernEntry);
			}
		}

		res.loaded = true;
	}
	PopMemoryPool(temp_memory, pop);

	return res;
}

internal i32
GetFontIndex_(FontBAG font_bag, const char *name)
{
	i32 res = -1;

	if(font_bag.fonts)
	{
		for (i32 i = 0; i < font_bag.font_count; ++i)
		{
			if(AreEqual(font_bag.fonts[i].name, name))
			{
				res = i;
				break;
			}
		}
	}

	return res;
}

internal Glyph
GetGlyph_(Font font, i32 unicode_codepoint)
{
	Glyph res = {};

	if((unicode_codepoint <= 0xffff) && font.glyphs)
	{
		i32 glyph_count = (i32)font.glyph_count;

		i32 l = 0;
		i32 r = glyph_count;
		while (l <= r)
		{
			i32 m = l + (r-l)/2;

			if (font.glyphs[m].unicode_codepoint == unicode_codepoint) 
			{
				res = font.glyphs[m];
				break;	
			}

			if (font.glyphs[m].unicode_codepoint < unicode_codepoint) 
				l = m + 1; 
			else 
				r = m - 1; 
		}
	}

	return res;
}
#define GetGlyph(font,codepoint) GetGlyph_(font,(i32)codepoint)

internal i32
FindGlyphIndex_(Font font, i32 unicode_codepoint)
{
	i32 res = -1;

	if((unicode_codepoint <= 0xffff) && font.glyphs)
	{
		i32 glyph_count = (i32)font.glyph_count;

		i32 l = 0;
		i32 r = (i32)glyph_count;
		while (l <= r)
		{
			i32 m = l + (r-l)/2;

			if (font.glyphs[m].unicode_codepoint == unicode_codepoint) 
			{
				res = m;
				break;	
			}

			if (font.glyphs[m].unicode_codepoint < unicode_codepoint) 
				l = m + 1; 
			else 
				r = m - 1; 
		}
	}

	return res;
}
#define FindGlyphIndex(font,codepoint) FindGlyphIndex_(font,(i32)codepoint)

inline i32
GetKernAdvance(Font font, i32 unicode_codepoint, i32 unicode_codepoint_1)
{
	i32 res = 0;

	if((unicode_codepoint <= 0xffff) && (unicode_codepoint_1 <= 0xffff) && font.kern)
	{
		u32 kern_count = font.kern_count;

		for (int i = 0; i < kern_count; ++i)
		{
			if((font.kern[i].glyph1_codepoint == unicode_codepoint) && 
					(font.kern[i].glyph2_codepoint == unicode_codepoint_1))
			{
				res = font.kern[i].kern_advance;						
				break;
			}
		}
		// u32 l = 0;
		// u32 r = kern_count;
		// while (l <= r)
		// {
		// 	u32 m = l + (r-l)/2;

		// 	if (font.kern[m].glyph1_codepoint == unicode_codepoint) 
		// 	{
		// 		i32 next = m;
		// 		while(next >= 0)
		// 		{
		// 			if((font.kern[next].glyph1_codepoint == unicode_codepoint) && 
		// 					(font.kern[next].glyph2_codepoint == unicode_codepoint_1))
		// 			{
		// 				res = font.kern[next].kern_advance;						
		// 				break;
		// 			}
		// 			next = (next+1 <= r ? next+1 : -1);
		// 		}
		// 		break;
		// 	}

		// 	if (font.kern[m].glyph1_codepoint < unicode_codepoint) 
		// 		l = m + 1; 
		// 	else 
		// 		r = m - 1; 
		// }
	}

	return res;
}

internal void 
AddPoint(V2 *points, i32 n, r32 x, r32 y)
{
	if (!points) return; // during first pass, it's unallocated
	points[n].x = x;
	points[n].y = y;
}

internal i32 
TesselateCurve(V2 *points, i32 *num_points, r32 x0, r32 y0, r32 x1, r32 y1, r32 x2, r32 y2, r32 objspace_flatness_squared, i32 n)
{
	// midpoint
	r32 mx = (x0 + 2*x1 + x2)/4;
	r32 my = (y0 + 2*y1 + y2)/4;
	// versus directly drawn line
	r32 dx = (x0+x2)/2 - mx;
	r32 dy = (y0+y2)/2 - my;
	if (n > 16) // 65536 segments on one curve better be enough!
		return 1;
	if (dx*dx+dy*dy > objspace_flatness_squared) 
	{ // half-pixel error allowed... need to be smaller if AA
		TesselateCurve(points, num_points, x0,y0, (x0+x1)/2.0f,(y0+y1)/2.0f, mx,my, objspace_flatness_squared,n+1);
		TesselateCurve(points, num_points, mx,my, (x1+x2)/2.0f,(y1+y2)/2.0f, x2,y2, objspace_flatness_squared,n+1);
	} 
	else 
	{
		AddPoint(points, *num_points,x2,y2);
		*num_points = *num_points+1;
	}
	return 1;
}

internal void 
TesselateCubic(V2 *points, i32 *num_points, r32 x0, r32 y0, r32 x1, r32 y1, r32 x2, r32 y2, r32 x3, r32 y3, r32 objspace_flatness_squared, i32 n)
{
	// @TODO this "flatness" calculation is just made-up nonsense that seems to work well enough
	r32 dx0 = x1-x0;
	r32 dy0 = y1-y0;
	r32 dx1 = x2-x1;
	r32 dy1 = y2-y1;
	r32 dx2 = x3-x2;
	r32 dy2 = y3-y2;
	r32 dx = x3-x0;
	r32 dy = y3-y0;
	r32 longlen = (r32) (sqrtf(dx0*dx0+dy0*dy0)+sqrtf(dx1*dx1+dy1*dy1)+sqrtf(dx2*dx2+dy2*dy2));
	r32 shortlen = (r32) sqrtf(dx*dx+dy*dy);
	r32 flatness_squared = longlen*longlen-shortlen*shortlen;

	if (n > 16) // 65536 segments on one curve better be enough!
		return;

	if (flatness_squared > objspace_flatness_squared) 
	{
		r32 x01 = (x0+x1)/2;
		r32 y01 = (y0+y1)/2;
		r32 x12 = (x1+x2)/2;
		r32 y12 = (y1+y2)/2;
		r32 x23 = (x2+x3)/2;
		r32 y23 = (y2+y3)/2;

		r32 xa = (x01+x12)/2;
		r32 ya = (y01+y12)/2;
		r32 xb = (x12+x23)/2;
		r32 yb = (y12+y23)/2;

		r32 mx = (xa+xb)/2;
		r32 my = (ya+yb)/2;

		TesselateCubic(points, num_points, x0,y0, x01,y01, xa,ya, mx,my, objspace_flatness_squared,n+1);
		TesselateCubic(points, num_points, mx,my, xb,yb, x23,y23, x3,y3, objspace_flatness_squared,n+1);
	} 
	else 
	{
		AddPoint(points, *num_points,x3,y3);
		*num_points = *num_points+1;
	}
}

inline void 
GetGlyphBitmapBoxSubpixel(Glyph glyph, r32 scale_x, r32 scale_y, i32 *ix0, i32 *iy0, i32 *ix1, i32 *iy1)
{
	// move to integral bboxes (treating pixels as little squares, what pixels get touched)?
	if (ix0) *ix0 = (i32)floor( glyph.min_x * scale_x);
	if (iy0) *iy0 = (i32)floor(-glyph.max_y * scale_y);
	if (ix1) *ix1 = (i32)ceil ( glyph.max_x * scale_x);
	if (iy1) *iy1 = (i32)ceil (-glyph.min_y * scale_y);
}

internal V2 *
FlattenCurves(FontVertex *vertices, i32 num_verts, r32 objspace_flatness, i32 **contour_lengths, i32 *num_contours, MemoryPool *temp_memory)
{
	V2 *points=0;
	i32 num_points=0;

	r32 objspace_flatness_squared = objspace_flatness * objspace_flatness;
	i32 i,n=0,start=0, pass;

	// count how many "moves" there are to get the contour count
	for (i=0; i < num_verts; ++i)
		if (vertices[i].type == vmove)
			++n;

	*num_contours = n;
	if (n == 0) return 0;

	*contour_lengths = AllocateFromMemoryPool(temp_memory,i32,n);
	if (*contour_lengths == 0) 
	{
		*num_contours = 0;
		return 0;
	}

	// make two passes through the points so we don't need to realloc
	for (pass=0; pass < 2; ++pass) 
	{
		r32 x=0,y=0;
		if (pass == 1) 
		{
			points = AllocateFromMemoryPool(temp_memory,V2,num_points);
			if (points == NULL) goto error;
		}
		num_points = 0;
		n= -1;
		for (i=0; i < num_verts; ++i) 
		{
			switch (vertices[i].type) 
			{
				case vmove:
				// start the next contour
				{
					if (n >= 0)
					(*contour_lengths)[n] = num_points - start;
					++n;
					start = num_points;

					x = vertices[i].x, y = vertices[i].y;
					AddPoint(points, num_points++, x,y);
				} break;
				case vline:
				{
					x = vertices[i].x, y = vertices[i].y;
					AddPoint(points, num_points++, x, y);
				} break;
				case vcurve:
				{
					TesselateCurve(points, &num_points, x,y,
					vertices[i].cx, vertices[i].cy,
					vertices[i].x,  vertices[i].y,
					objspace_flatness_squared, 0);
					x = vertices[i].x, y = vertices[i].y;
				} break;
				case vcubic:
				{
					TesselateCubic(points, &num_points, x,y,
					vertices[i].cx, vertices[i].cy,
					vertices[i].cx1, vertices[i].cy1,
					vertices[i].x,  vertices[i].y,
					objspace_flatness_squared, 0);
					x = vertices[i].x, y = vertices[i].y;
				} break;
			}
		}
		(*contour_lengths)[n] = num_points - start;
	}

	return points;

	error:
	*contour_lengths = 0;
	*num_contours = 0;
	return NULL;
}

struct Edge
{
	r32 x0,y0, x1,y1;
	i32 invert;
};

#define COMPARE_EDGES(a,b)  ((a)->y0 < (b)->y0)

internal void 
SortEdgesInsertionSort(Edge *p, i32 n)
{
	i32 i,j;
	for (i=1; i < n; ++i) 
	{
		Edge t = p[i], *a = &t;
		j = i;
		while (j > 0) 
		{
			Edge *b = &p[j-1];
			i32 c = COMPARE_EDGES(a,b);
			if (!c) break;
			p[j] = p[j-1];
			--j;
		}
		if (i != j)
			p[j] = t;
	}
}

internal void 
SortEdgesQuickSort(Edge *p, i32 n)
{
	/* threshhold for transitioning to insertion sort */
	while (n > 12) 
	{
		Edge t;
		i32 c01,c12,c,m,i,j;

		/* compute median of three */
		m = n >> 1;
		c01 = COMPARE_EDGES(&p[0],&p[m]);
		c12 = COMPARE_EDGES(&p[m],&p[n-1]);
		/* if 0 >= mid >= end, or 0 < mid < end, then use mid */
		if (c01 != c12) 
		{
			/* otherwise, we'll need to swap something else to middle */
			i32 z;
			c = COMPARE_EDGES(&p[0],&p[n-1]);
			/* 0>mid && mid<n:  0>n => n; 0<n => 0 */
			/* 0<mid && mid>n:  0>n => 0; 0<n => n */
			z = (c == c12) ? 0 : n-1;
			t = p[z];
			p[z] = p[m];
			p[m] = t;
		}
		/* now p[m] is the median-of-three */
		/* swap it to the beginning so it won't move around */
		t = p[0];
		p[0] = p[m];
		p[m] = t;

		/* partition loop */
		i=1;
		j=n-1;
		for(;;) {
			/* handling of equality is crucial here */
			/* for sentinels & efficiency with duplicates */
			for (;;++i) 
			{
				if (!COMPARE_EDGES(&p[i], &p[0])) break;
			}
			for (;;--j) 
			{
				if (!COMPARE_EDGES(&p[0], &p[j])) break;
			}
			/* make sure we haven't crossed */
			if (i >= j) break;
			t = p[i];
			p[i] = p[j];
			p[j] = t;

			++i;
			--j;
		}
		/* recurse on smaller side, iterate on larger */
		if (j < (n-i)) 
		{
			SortEdgesQuickSort(p,j);
			p = p+i;
			n = n-i;
		} 
		else 
		{
			SortEdgesQuickSort(p+i, n-i);
			n = j;
		}
	}
}

internal void 
SortEdges(Edge *p, i32 n)
{
	SortEdgesQuickSort(p, n);
	SortEdgesInsertionSort(p, n);
}

struct HheapChunk
{
	HheapChunk *next;
};

struct Hheap
{
	HheapChunk *head;
	void   *first_free;
	i32    num_remaining_in_head_chunk;
};

internal void *
HheapAlloc(Hheap *hh, size_t size, MemoryPool *temp_memory)
{
	if (hh->first_free) 
	{
		void *p = hh->first_free;
		hh->first_free = * (void **) p;
		return p;
	} 
	else 
	{
		if (hh->num_remaining_in_head_chunk == 0) 
		{
			i32 count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
			HheapChunk *c = (HheapChunk *) AllocateFromMemoryPool_(temp_memory, (sizeof(HheapChunk) + size * count));
			if (c == NULL)
				return NULL;
			c->next = hh->head;
			hh->head = c;
			hh->num_remaining_in_head_chunk = count;
		}

		--hh->num_remaining_in_head_chunk;
		return (char *) (hh->head) + size * hh->num_remaining_in_head_chunk;
	}
}

internal void 
HheapFree(Hheap *hh, void *p)
{
	*(void **) p = hh->first_free;
	hh->first_free = p;
}

internal void 
HheapCleanup(Hheap *hh)
{
	HheapChunk *c = hh->head;
	while (c) 
	{
		HheapChunk *n = c->next;
		c = n;
	}
}

struct ActiveEdge
{
	ActiveEdge *next;
	r32 fx,fdx,fdy;
	r32 direction;
	r32 sy;
	r32 ey;
};

internal ActiveEdge *
NewActive(Hheap *hh, Edge *e, i32 off_x, r32 start_point, MemoryPool *temp_memory)
{
	ActiveEdge *z = (ActiveEdge *) HheapAlloc(hh, sizeof(*z), temp_memory);
	r32 dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
	assert(z != NULL);
	//assert(e->y0 <= start_point);
	if (!z) return z;
	z->fdx = dxdy;
	z->fdy = dxdy != 0.0f ? (1.0f/dxdy) : 0.0f;
	z->fx = e->x0 + dxdy * (start_point - e->y0);
	z->fx -= off_x;
	z->direction = e->invert ? 1.0f : -1.0f;
	z->sy = e->y0;
	z->ey = e->y1;
	z->next = 0;
	return z;
}

// the edge passed in here does not cross the vertical line at x or the vertical line at x+1
// (i.e. it has already been clipped to those)
internal void 
HandleClippedEdge(r32 *scanline, i32 x, ActiveEdge *e, r32 x0, r32 y0, r32 x1, r32 y1)
{
	if (y0 == y1) return;
	assert(y0 < y1);
	assert(e->sy <= e->ey);
	if (y0 > e->ey) return;
	if (y1 < e->sy) return;
	if (y0 < e->sy) 
	{
		x0 += (x1-x0) * (e->sy - y0) / (y1-y0);
		y0 = e->sy;
	}
	if (y1 > e->ey) 
	{
		x1 += (x1-x0) * (e->ey - y1) / (y1-y0);
		y1 = e->ey;
	}

	if (x0 == x)
	{
		assert(x1 <= x+1);
	}
	else if (x0 == x+1)
	{
		assert(x1 >= x);
	}
	else if (x0 <= x)
	{
		assert(x1 <= x);
	}	
	else if (x0 >= x+1)
	{
		assert(x1 >= x+1);
	}
	else
		assert(x1 >= x && x1 <= x+1);

	if (x0 <= x && x1 <= x)
		scanline[x] += e->direction * (y1-y0);
	else if (x0 >= x+1 && x1 >= x+1)
	{}
	else 
	{
		assert(x0 >= x && x0 <= x+1 && x1 >= x && x1 <= x+1);
		scanline[x] += e->direction * (y1-y0) * (1-((x0-x)+(x1-x))/2); // coverage = 1 - average x position
	}
}

internal void 
FillActiveEdgesNew(r32 *scanline, r32 *scanline_fill, i32 len, ActiveEdge *e, r32 y_top)
{
	r32 y_bottom = y_top+1;

	while (e) 
	{
		// brute force every pixel

		// compute intersection points with top & bottom
		assert(e->ey >= y_top);

		if (e->fdx == 0) 
		{
			r32 x0 = e->fx;
			if (x0 < len) 
			{
				if (x0 >= 0) 
				{
					HandleClippedEdge(scanline,(i32) x0,e, x0,y_top, x0,y_bottom);
					HandleClippedEdge(scanline_fill-1,(i32) x0+1,e, x0,y_top, x0,y_bottom);
				} 
				else 
				{
					HandleClippedEdge(scanline_fill-1,0,e, x0,y_top, x0,y_bottom);
				}
			}
		} 
		else 
		{
			r32 x0 = e->fx;
			r32 dx = e->fdx;
			r32 xb = x0 + dx;
			r32 x_top, x_bottom;
			r32 sy0,sy1;
			r32 dy = e->fdy;
			assert(e->sy <= y_bottom && e->ey >= y_top);

			// compute endpoints of line segment clipped to this scanline (if the
			// line segment starts on this scanline. x0 is the intersection of the
			// line with y_top, but that may be off the line segment.
			if (e->sy > y_top) 
			{
				x_top = x0 + dx * (e->sy - y_top);
				sy0 = e->sy;
			} 
			else 
			{
				x_top = x0;
				sy0 = y_top;
			}
			if (e->ey < y_bottom) 
			{
				x_bottom = x0 + dx * (e->ey - y_top);
				sy1 = e->ey;
			} 
			else 
			{
				x_bottom = xb;
				sy1 = y_bottom;
			}

			if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) 
			{
				// from here on, we don't have to range check x values
				if ((i32) x_top == (i32) x_bottom) 
				{
					r32 height;
					// simple case, only spans one pixel
					i32 x = (i32) x_top;
					height = sy1 - sy0;
					assert(x >= 0 && x < len);
					scanline[x] += e->direction * (1-((x_top - x) + (x_bottom-x))/2)  * height;
					scanline_fill[x] += e->direction * height; // everything right of this pixel is filled
				} 
				else 
				{
					i32 x,x1,x2;
					r32 y_crossing, step, sign, area;
					// covers 2+ pixels
					if (x_top > x_bottom) 
					{
						// flip scanline vertically; signed area is the same
						r32 t;
						sy0 = y_bottom - (sy0 - y_top);
						sy1 = y_bottom - (sy1 - y_top);
						t = sy0, sy0 = sy1, sy1 = t;
						t = x_bottom, x_bottom = x_top, x_top = t;
						dx = -dx;
						dy = -dy;
						t = x0, x0 = xb, xb = t;
					}

					x1 = (i32) x_top;
					x2 = (i32) x_bottom;
					// compute intersection with y axis at x1+1
					y_crossing = (x1+1 - x0) * dy + y_top;

					sign = e->direction;
					// area of the rectangle covered from y0..y_crossing
					area = sign * (y_crossing-sy0);
					// area of the triangle (x_top,y0), (x+1,y0), (x+1,y_crossing)
					scanline[x1] += area * (1-((x_top - x1)+(x1+1-x1))/2);

					step = sign * dy;
					for (x = x1+1; x < x2; ++x) 
					{
						scanline[x] += area + step/2;
						area += step;
					}
					y_crossing += dy * (x2 - (x1+1));

					assert(fabs(area) <= 1.01f);

					scanline[x2] += area + sign * (1-((x2-x2)+(x_bottom-x2))/2) * (sy1-y_crossing);

					scanline_fill[x2] += sign * (sy1-sy0);
				}
			} 
			else 
			{
				// if edge goes outside of box we're drawing, we require
				// clipping logic. since this does not match the intended use
				// of this library, we use a different, very slow brute
				// force implementation
				i32 x;
				for (x=0; x < len; ++x) 
				{
					// cases:
					//
					// there can be up to two intersections with the pixel. any intersection
					// with left or right edges can be handled by splitting into two (or three)
					// regions. intersections with top & bottom do not necessitate case-wise logic.
					//
					// the old way of doing this found the intersections with the left & right edges,
					// then used some simple logic to produce up to three segments in sorted order
					// from top-to-bottom. however, this had a problem: if an x edge was epsilon
					// across the x border, then the corresponding y position might not be distinct
					// from the other y segment, and it might ignored as an empty segment. to avoid
					// that, we need to explicitly produce segments based on x positions.

					// rename variables to clearly-defined pairs
					r32 y0 = y_top;
					r32 x1 = (r32) (x);
					r32 x2 = (r32) (x+1);
					r32 x3 = xb;
					r32 y3 = y_bottom;

					// x = e->x + e->dx * (y-y_top)
					// (y-y_top) = (x - e->x) / e->dx
					// y = (x - e->x) / e->dx + y_top
					r32 y1 = (x - x0) / dx + y_top;
					r32 y2 = (x+1 - x0) / dx + y_top;

					if (x0 < x1 && x3 > x2) 
					{         // three segments descending down-right
						HandleClippedEdge(scanline,x,e, x0,y0, x1,y1);
						HandleClippedEdge(scanline,x,e, x1,y1, x2,y2);
						HandleClippedEdge(scanline,x,e, x2,y2, x3,y3);
					} 
					else if (x3 < x1 && x0 > x2) 
					{  // three segments descending down-left
						HandleClippedEdge(scanline,x,e, x0,y0, x2,y2);
						HandleClippedEdge(scanline,x,e, x2,y2, x1,y1);
						HandleClippedEdge(scanline,x,e, x1,y1, x3,y3);
					} 
					else if (x0 < x1 && x3 > x1) 
					{  // two segments across x, down-right
						HandleClippedEdge(scanline,x,e, x0,y0, x1,y1);
						HandleClippedEdge(scanline,x,e, x1,y1, x3,y3);
					} 
					else if (x3 < x1 && x0 > x1) 
					{  // two segments across x, down-left
						HandleClippedEdge(scanline,x,e, x0,y0, x1,y1);
						HandleClippedEdge(scanline,x,e, x1,y1, x3,y3);
					} 
					else if (x0 < x2 && x3 > x2) 
					{  // two segments across x+1, down-right
						HandleClippedEdge(scanline,x,e, x0,y0, x2,y2);
						HandleClippedEdge(scanline,x,e, x2,y2, x3,y3);
					} 
					else if (x3 < x2 && x0 > x2) 
					{  // two segments across x+1, down-left
						HandleClippedEdge(scanline,x,e, x0,y0, x2,y2);
						HandleClippedEdge(scanline,x,e, x2,y2, x3,y3);
					} 
					else 
					{  // one segment
						HandleClippedEdge(scanline,x,e, x0,y0, x3,y3);
					}
				}
			}
		}

		e = e->next;
	}
}

// directly AA rasterize edges w/o supersampling
internal void 
RasterizeSortedEdges(u32 bmp_width, u32 bmp_height, u8 *pixel_data, Edge *e, i32 n, i32 vsubsample, i32 off_x, i32 off_y, MemoryPool *temp_memory)
{
	Hheap hh = { 0, 0, 0 };
	ActiveEdge *active = NULL;
	i32 y,j=0, i;
	r32 scanline_data[129], *scanline, *scanline2;

	if (bmp_width > 64)
		scanline = AllocateFromMemoryPool(temp_memory,r32,(bmp_width*2+1));
	else
		scanline = scanline_data;

	scanline2 = scanline + bmp_width;

	y = off_y;
	e[n].y0 = (r32) (off_y + bmp_height) + 1;

	while (j < bmp_height) 
	{
		// find center of pixel for this scanline
		r32 scan_y_top    = y + 0.0f;
		r32 scan_y_bottom = y + 1.0f;
		ActiveEdge **step = &active;

		memset(scanline , 0, bmp_width*sizeof(scanline[0]));
		memset(scanline2, 0, (bmp_width+1)*sizeof(scanline[0]));

		// update all active edges;
		// remove all active edges that terminate before the top of this scanline
		while (*step) 
		{
			ActiveEdge * z = *step;
			if (z->ey <= scan_y_top) 
			{
				*step = z->next; // delete from list
				assert(z->direction);
				z->direction = 0;
				HheapFree(&hh, z);
			} 
			else 
			{
				step = &((*step)->next); // advance through list
			}
		}

		// insert all edges that start before the bottom of this scanline
		while (e->y0 <= scan_y_bottom) 
		{
			if (e->y0 != e->y1) 
			{
				ActiveEdge *z = NewActive(&hh, e, off_x, scan_y_top, temp_memory);
				if (z != NULL) 
				{
					assert(z->ey >= scan_y_top);
					// insert at front
					z->next = active;
					active = z;
				}
			}
			++e;
		}

		// now process all active edges
		if (active)
		FillActiveEdgesNew(scanline, scanline2+1, bmp_width, active, scan_y_top);

		{
			r32 sum = 0;
			for (i=0; i < bmp_width; ++i) 
			{
				r32 k;
				i32 m;
				sum += scanline2[i];
				k = scanline[i] + sum;
				k = (r32) fabs(k)*255 + 0.5f;
				m = (i32) k;
				if (m > 255) m = 255;
				pixel_data[j*bmp_width + i] = (u8) m;
			}
		}
		// advance all the edges
		step = &active;
		while (*step) 
		{
			ActiveEdge *z = *step;
			z->fx += z->fdx; // advance to position for current scanline
			step = &((*step)->next); // advance through list
		}

		++y;
		++j;
	}

	HheapCleanup(&hh);
}

internal void 
RasterizeWindings(u32 bmp_width, u32 bmp_height, u8 *pixel_data, V2 *pts, i32 *wcount, i32 windings, 
				  r32 scale_x, r32 scale_y, i32 off_x, i32 off_y, i32 invert, MemoryPool *temp_memory)
{
	r32 y_scale_inv = invert ? -scale_y : scale_y;
	Edge *e;
	i32 n,i,j,k,m;
	i32 vsubsample = 1;
	// vsubsample should divide 255 evenly; otherwise we won't reach full opacity

	// now we have to blow out the windings into explicit edge lists
	n = 0;
	for (i=0; i < windings; ++i)
		n += wcount[i];

	e = AllocateFromMemoryPool(temp_memory,Edge,(n+1)); // add an extra one as a sentinel
	if (e == 0) return;
	n = 0;

	m=0;
	for (i=0; i < windings; ++i) 
	{
		V2 *p = pts + m;
		m += wcount[i];
		j = wcount[i]-1;
		for (k=0; k < wcount[i]; j=k++) 
		{
			i32 a=k,b=j;
			// skip the edge if horizontal
			if (p[j].y == p[k].y)
			continue;
			// add edge from j to k to the list
			e[n].invert = 0;
			if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) 
			{
				e[n].invert = 1;
				a=j,b=k;
			}
			e[n].x0 = p[a].x * scale_x;
			e[n].y0 = (p[a].y * y_scale_inv) * vsubsample;
			e[n].x1 = p[b].x * scale_x;
			e[n].y1 = (p[b].y * y_scale_inv) * vsubsample;
			++n;
		}
	}

	// now sort the edges by their highest point (should snap to integer, and then by x)
	//sort(e, n, sizeof(e[0]), _edge_compare);
	SortEdges(e, n);

	// now, traverse the scanlines and find the intersections on each scanline, use xor winding rule
	RasterizeSortedEdges(bmp_width, bmp_height, pixel_data, e, n, vsubsample, off_x, off_y, temp_memory);
}

void 
Rasterize(u32 bmp_width, u32 bmp_height, u8 *pixel_data, r32 flatness_in_pixels, FontVertex *vertices, i32 num_verts, 
		   r32 scale_x, r32 scale_y, i32 off_x, i32 off_y, i32 invert, MemoryPool *temp_memory)
{
	r32 scale = scale_x > scale_y ? scale_y : scale_x;
	i32 winding_count, *winding_lengths;
	V2 *windings = FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, temp_memory);
	if (windings) 
	{
		RasterizeWindings(bmp_width, bmp_height, pixel_data, windings, winding_lengths, winding_count, scale_x, scale_y, off_x, off_y, invert, temp_memory);
	}
}

internal u8 *
GetGlyphBitmapSubpixel(Font font, r32 scale_x, r32 scale_y, Glyph glyph, u32 *width, u32 *height, MemoryPool *memory, MemoryPool *temp_memory)
{
	u8 *res = 0;
	i32 ix0,iy0,ix1,iy1;
	FontVertex *vertices = glyph.vertices;
	i32 num_verts = glyph.vertex_count;

	if (scale_x == 0) scale_x = scale_y;
	if (scale_y == 0) 
	{
		if (scale_x == 0) 
		{
			return 0;
		}
		scale_y = scale_x;
	}

	if(glyph.unicode_codepoint == 237)
		glyph.unicode_codepoint = 237;

	GetGlyphBitmapBoxSubpixel(glyph, scale_x, scale_y, &ix0,&iy0,&ix1,&iy1);

	// now we get the size
	if (width ) *width  = (ix1 - ix0);
	if (height) *height = (iy1 - iy0);

	if (*width && *height) 
	{
		// TODO: Desalocar qdo baking em tamanho diferente
		u8 * pop = PushMemoryPool(temp_memory);
		
		u32 size = (*width)*(*height);
		u8 *alpha_pixels = AllocateFromMemoryPool(temp_memory,u8,size);
		if (alpha_pixels) 
		{
			Rasterize(*width, *height, alpha_pixels, 0.35f, vertices, num_verts, scale_x, scale_y, ix0, iy0, 1, temp_memory);
			
			res = AllocateFromMemoryPool(memory,u8,size*4);
			if(res)
			{
				u32 j = 0;
				for (i32 y = (*height)-1; y >= 0; --y)
				{
					for (i32 x = 0; x < (*width); ++x)
					{
						i32 i = y*(*width) + x;
						res[j++] = alpha_pixels[i];
						res[j++] = alpha_pixels[i];
						res[j++] = alpha_pixels[i];
						res[j++] = alpha_pixels[i];
					}
				}
			}
		}

		PopMemoryPool(temp_memory, pop);
	}

	return res;
}

internal i32 
GetUnicodeCodepointFromUTF8(char *c, i32 *sub)
{
	*sub = 0;
	char mask = 0b10000000;
	i32 unicode_mask = 0b11111111;
	if((*c) & mask) 
	{
		*(sub) += 1;
		mask = 0b01000000;
		if((*c) & mask)
		{
			*(sub) += 1;
			mask = 0b00100000;
			if((*c) & mask) 
			{
				*(sub) += 1;
				mask = 0b00010000;
				if((*c) & mask)
				{
					*(sub) += 1;
				} 
			}
		}
	}
	else 
	{
		*sub = 1;
	}

	i32 mask_ = (*sub==1 ? 0xff : 
								(*sub==2 ? 0xffff : 
									(*sub==3 ? 0xffffff : 0xffffffff)));

	i32 utf_8 = U32Swap((((*((i32*)c)) << (32 - (*sub)*8)) >> (32 - (*sub)*8)) & mask_) >> (32-(*sub)*8);
	i32 res = 0;
	if(*sub == 1)
	{
		res = utf_8 & 0b01111111;
	}
	else if(*sub == 2)
	{
		// 0b1100 xxxx 10xx xxxx
		i32 a = ((utf_8 & 0b0000111100000000) >> 2) & 0b0000001111000000;
		i32 b = (utf_8                              & 0b0000000000111111);
		res = a | b;
	}
	else if(*sub == 3)
	{
		// 1110 xxxx 10xx xxxx 10xx xxxx
		i32 a = ((utf_8 & 0b000011110000000000000000) >> 4) & 0b000000001111000000000000; 
		i32 b = ((utf_8 & 0b000000000011111100000000) >> 2) & 0b000000000000111111000000; 
		i32 c = (utf_8                                      & 0b000000000000000000111111);
		res = a | b | c;
	}
	else if(*sub == 4)
	{
		// 1111 xxxx 10xx xxxx 10xx xxxx 10xx xxxx
		i32 a = ((utf_8 & 0b00001111000000000000000000000000) >> 6) & 0b00000000001111000000000000000000; 
		i32 b = ((utf_8 & 0b00000000001111110000000000000000) >> 4) & 0b00000000000000111111000000000000; 
		i32 c = ((utf_8 & 0b00000000000000000011111100000000) >> 2) & 0b00000000000000000000111111000000;
		i32 d = (utf_8                                              & 0b00000000000000000000000000111111);
		res = a | b | c | d;
	}
	return res;
}

inline r32
AdvanceLine(Font *font, r32 wsize)
{
	r32 res = (r32)(font->line_gap*wsize) / font->font_height;
	res += wsize;
	return res;
}

internal V2
RenderText (RendererState *renderer_state, Font *font, Mat4 vp, char *text, u32 size, r32 wsize, V2 point, V3 color, r32 alpha, MemoryPool *memory, MemoryPool *temp_memory)
{
	V2 res = point;
	r32 scale_y = (r32) size / font->font_height;

	u32 length = GetLength(text);
	for (i32 i = 0; i < length; ++i)
	{
		char *c = text + i;
		i32 sub;
		i32 unicode_codepoint = GetUnicodeCodepointFromUTF8(c,&sub);
		
		i32 glyph_index = FindGlyphIndex(*font,unicode_codepoint);
		if(glyph_index < 0)
		{
			// TODO: show no glyph
			if(unicode_codepoint == 10)
			{
				point.x = res.x;
				point.y -= AdvanceLine(font,wsize);
			}
		}
		else
		{
			Glyph *glyph = &font->glyphs[glyph_index];

			if(glyph->id && glyph->w && glyph->h && glyph->bitmap && (size == glyph->size))
			{
				// Ja rasterizado no tamanho certo
			}
			else
			{
				glyph->bitmap = GetGlyphBitmapSubpixel(*font, 0.0f,scale_y, *glyph, &glyph->w, &glyph->h, memory, temp_memory);
				if(glyph->bitmap)
					RendererRGBABitmapToGPU(&glyph->id, glyph->w, glyph->h, glyph->bitmap);
				glyph->size = size;
			}

			i32 g_height = glyph->max_y - glyph->min_y;
			i32 g_width = glyph->max_x - glyph->min_x;
			i32 advance_x = glyph->advance_width;

			if(g_width && g_height)
			{
				r32 size_y = (r32)(g_height*wsize)/font->font_height;
				r32 size_x = (r32)(g_width*size_y)/g_height;

				r32 base_x = point.x + ((r32)glyph->min_x*size_x / g_width);//(r32)(point.x * (glyph->min_x)) / font->min_x;
				r32 base_y = point.y + ((r32)glyph->min_y*size_y / g_height); //+ (r32)(font->ascent*wsize)/font->font_height;
				// if(font->descent)
				// {
				// 	r32 wdesc = (r32)(-font->descent*wsize) / font->font_height;
				// 	ApplyTransforms(&render_info->model_matrix, V3(point.x,point.y-wdesc, 0.0f), V3(0.1f,0.1f,1.0f), QUAT_ZERO);
				// 	RenderTexturedQuad((render_info->model_matrix*vp), 1.0f, glyph->id, true);
				// 	r32 wasc = (r32)(font->ascent*wsize) / font->font_height;
				// 	ApplyTransforms(&render_info->model_matrix, V3(point.x,point.y+wasc, 0.0f), V3(0.1f,0.1f,1.0f), QUAT_ZERO);
				// 	RenderTexturedQuad((render_info->model_matrix*vp), 1.0f, glyph->id, true);
				// 	// base_y = (r32)(glyph->min_y*(point.y-wdesc))/font->descent;
				// }
				V3 center = V3(base_x+(size_x*0.5f), base_y+(size_y*0.5f),0.0f);
				V3 scale = V3(size_x,size_y,0.0f);
				Mat4 model_matrix = {};
				ApplyTransforms(&model_matrix, center, scale, QUAT_ZERO);
				RenderTexturedQuad(renderer_state, (model_matrix*vp), alpha, color, glyph->id, false);

				// ApplyTransforms(&render_info->model_matrix, V2_V3(point,0.0f), V3(0.1f,0.1f,0.1f), QUAT_ZERO);
				// RenderTexturedQuad((render_info->model_matrix*vp), 1.0f, 0, true);

				if(i < length-1)
				{
					i32 sub1;
					i32 unicode_codepoint_1 = GetUnicodeCodepointFromUTF8(c+1, &sub1);
					glyph_index = FindGlyphIndex(*font, unicode_codepoint_1);
					if(glyph_index >= 0)
					{
						Glyph next = font->glyphs[glyph_index];
						advance_x += GetKernAdvance(*font, unicode_codepoint, unicode_codepoint_1);
					}
				}
				
				point.x += (r32)(advance_x*size_x)/g_width;
			}
			else point.x += (r32)advance_x*wsize/font->font_height;
		}

		i += sub-1;
  }

  res = point;
  return res;
}

// internal void
// LoadOutlineFont(GameState *game_state, const char *filename)
// {
// 	OutlineFont *font = &game_state->font;
// 	font->char_size = V2(0.6,1.0f) * 2.0f;

// 	RawFileContents bagsvf_file = PlatformGetFileContents(filename);
// 	void *p = bagsvf_file.contents;
// 	if(bagsvf_file.contents)
// 	{
// 		for (u32 i = 0; i < 26; i++)
// 		{
// 			LineSet *set = &font->uppercase[i];
// 			u32 set_count = *(u32*)p;
// 			p = (void*)((u32*)p+1);
// 			set->set_count = set_count;
// 			set->point_count = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			set->ids = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			for (u32 j = 0; j < set_count; j++)
// 			{
// 				u32 point_count = *((u32*)p);
// 				p = (void*)((u32*)p+1);
// 				set->point_count[j] = point_count;

// 				V2 *points = (V2*)p;
// 				set->ids[j] = PointDataToGPU(points, point_count);				

// 				p = (void*)((V2*)p + point_count);
// 			}
// 		}
// 		for (u32 i = 0; i < 26; i++)
// 		{
// 			LineSet *set = &font->lowercase[i];
// 			u32 set_count = *(u32*)p;
// 			p = (void*)((u32*)p+1);
// 			set->set_count = set_count;
// 			set->point_count = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			set->ids = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			for (u32 j = 0; j < set_count; j++)
// 			{
// 				u32 point_count = *((u32*)p);
// 				p = (void*)((u32*)p+1);
// 				set->point_count[j] = point_count;

// 				V2 *points = (V2*)p;
// 				set->ids[j] = PointDataToGPU(points, point_count);				

// 				p = (void*)((V2*)p + point_count);
// 			}
// 		}
// 		for (u32 i = 0; i < 27; i++)
// 		{
// 			LineSet *set = &font->symbols[i];
// 			u32 set_count = *(u32*)p;
// 			p = (void*)((u32*)p+1);
// 			set->set_count = set_count;
// 			set->point_count = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			set->ids = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			for (u32 j = 0; j < set_count; j++)
// 			{
// 				u32 point_count = *((u32*)p);
// 				p = (void*)((u32*)p+1);
// 				set->point_count[j] = point_count;

// 				V2 *points = (V2*)p;
// 				set->ids[j] = PointDataToGPU(points, point_count);				

// 				p = (void*)((V2*)p + point_count);
// 			}
// 		}
// 		for (u32 i = 0; i < 10; i++)
// 		{
// 			LineSet *set = &font->numbers[i];
// 			u32 set_count = *(u32*)p;
// 			p = (void*)((u32*)p+1);
// 			set->set_count = set_count;
// 			set->point_count = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			set->ids = AllocateFromMemoryPool(&game_memory.permanent,u32,set_count);//(u32*)PlatformAlloc(set_count*sizeof(u32));
// 			for (u32 j = 0; j < set_count; j++)
// 			{
// 				u32 point_count = *((u32*)p);
// 				p = (void*)((u32*)p+1);
// 				set->point_count[j] = point_count;

// 				V2 *points = (V2*)p;
// 				set->ids[j] = PointDataToGPU(points, point_count);				

// 				p = (void*)((V2*)p + point_count);
// 			}
// 		}

// 		font->initialized = true;
// 	}
// }
