
#define PACK_SHAPE_ID(a,b,c,d) (((u32)(a)<<24)|((u32)(b)<<16)|((u32)(c)<<8)|((u32)(d)))

const u32 PATH_ID = PACK_SHAPE_ID('P','A','T','H');
struct VIPath
{
	char id[16];
	V2 min,max;
	V2 *points;
	u32 npoints;
	void *next;
};

const u32 CIRCLE_ID = PACK_SHAPE_ID('C','R','C','L');
struct VICircle
{
	char id[16];
	V2 center;
	r32 radius;
	void *next;
};

const u32 RECT_ID = PACK_SHAPE_ID('R','E','C','T');
struct VIRect
{
	char id[16];
	V2 p;
	r32 width, height;
	void *next;
};

const u32 RECT_ID = PACK_SHAPE_ID('G','R','U','P');
struct VIGroup
{
	char id[16];
	void *first_child;
	void *next;
};

struct VectorImage
{
	u32 width, height;
	void *first_shape;
};




internal void RasterizeVectorImage (VectorImage vector_image, u32 width, u32 height, r32 scale, u8 *pixels);