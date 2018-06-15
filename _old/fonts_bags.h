struct LineSet
{
	u32 set_count;
	u32 *point_count;
	u32 *ids;
};

// struct OutlineFont
// {
// 	V2 char_size;
// 	LineSet uppercase[26];
// 	LineSet lowercase[26];
// 	LineSet symbols[27];
// 	LineSet numbers[10];
// 	b32 initialized;
// };

struct FontVertex
{
	i16 x,y,cx,cy,cx1,cy1;
	u8 type,padding;// remover padding p/ packing?
};

enum 
{
	vmove=1,
	vline,
	vcurve,
	vcubic
};

struct Glyph
{
	FontVertex *vertices;
	u32 vertex_count;

	i32 unicode_codepoint;
	i32 advance_width;
	i32 left_bearing;

	i32 min_x;
	i32 min_y;
	i32 max_x;
	i32 max_y;

	u8 *bitmap;
	u32 size;
	u32 w,h;
	u32 id;
};

struct KernEntry
{
	i32 glyph1_codepoint;
	i32 glyph2_codepoint;
	i32 kern_advance;
};

struct Font
{
	char *name;

	KernEntry *kern;
	Glyph *glyphs;
	u32 glyph_count;
	u32 kern_count;

	i32 min_x;
	i32 min_y;
	i32 max_x;
	i32 max_y;
	i32 ascent;
	i32 descent;
	i32 line_gap;
	i32 font_height;
	u32 units_per_em;
};

struct FontBAG
{
	Font *fonts;
	u32 font_count;
	b32 loaded;
};

internal FontBAG GetFontBAG (const char *filename, MemoryPool *memory, MemoryPool *temp_memory);

internal i32 GetFontIndex_ (FontBAG font_bag, const char *name);
#define GetFontIndex(font_bag,name) GetFontIndex_((font_bag),(const char *)(name))
