
#pragma pack(push, 2)
struct BMPFileHeader
{
    char bf_type[2];
    s32 bf_size;
    u32 reserved;
    s32 bf_off_bits;
};
#pragma pack(pop)

struct BMPImageHeader
{
    u32 bi_size; // 40
    u32 bi_width;
    u32 bi_height;
    u16 bi_planes; // must be 1
    u16 bi_bit_count; // bits per pixel
    u32 bi_compression; // 0
    u32 bi_size_image; // can be 0 in uncompressed
    u32 bi_x_pels_per_meter; // preferred res in pixels per meter
    u32 bi_y_pels_per_meter; // preferred res in pixels per meter
    u32 bi_clr_used; // n of Color Map entries
    u32 bi_clr_important; // n of significant colors
};
#pragma pack(pop)

#define BITMAP_FORMAT_RGBA32 4

internal void
MakeBMP (u8 *memory, u32 width, u32 height, u32 format, u8 *pixels)
{
    if(memory && pixels)
    {
        u32 pixel_data_size = (width*height*format);
        u32 file_size = sizeof(BMPFileHeader) + sizeof(BMPImageHeader) + pixel_data_size;

        BMPFileHeader file_header = {{'B','M'}};
        file_header.bf_size = file_size;
        file_header.bf_off_bits = file_size-pixel_data_size;
        
        BMPImageHeader image_header = {};
        image_header.bi_size = sizeof(BMPImageHeader);
        image_header.bi_width = width;
        image_header.bi_height = height;
        image_header.bi_planes = 1;
        image_header.bi_bit_count = format*8;
        image_header.bi_size_image = pixel_data_size;

        u8 *src = (u8*)&file_header;
        for (u32 i = 0; i < sizeof(BMPFileHeader); ++i) *memory++ = src[i]; 
        
        src = (u8*)&image_header;
        for (u32 i = 0; i < sizeof(BMPImageHeader); ++i) *memory++ = src[i]; 
        
        for (u32 i = 0; i < pixel_data_size; ++i) *memory++ = pixels[i]; 
    }
}

internal u8 * 
MakeBMP (StaticMemory *memory, u32 width, u32 height, u32 format, u8 *pixels)
{
    u32 file_size = sizeof(BMPFileHeader) + sizeof(BMPImageHeader) + (width*height*format);
    u8 *res = AllocateStaticMemory_(memory, file_size);
    if(res)
    {
        MakeBMP(res, width, height, format, pixels);
    }
    return res;
}