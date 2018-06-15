
enum XCF_PROP
{
    PROP_END                =  0,
    PROP_COLORMAP           =  1,
    PROP_ACTIVE_LAYER       =  2,
    PROP_ACTIVE_CHANNEL     =  3,
    PROP_SELECTION          =  4,
    PROP_FLOATING_SELECTION =  5,
    PROP_OPACITY            =  6,
    PROP_MODE               =  7,
    PROP_VISIBLE            =  8,
    PROP_LINKED             =  9,
    PROP_LOCK_ALPHA         = 10,
    PROP_APPLY_MASK         = 11,
    PROP_EDIT_MASK          = 12,
    PROP_SHOW_MASK          = 13,
    PROP_SHOW_MASKED        = 14,
    PROP_OFFSETS            = 15,
    PROP_COLOR              = 16,
    PROP_COMPRESSION        = 17,
    PROP_GUIDES             = 18,
    PROP_RESOLUTION         = 19,
    PROP_TATTOO             = 20,
    PROP_PARASITES          = 21,
    PROP_UNIT               = 22,
    PROP_PATHS              = 23,
    PROP_USER_UNIT          = 24,
    PROP_VECTORS            = 25,
    PROP_TEXT_LAYER_FLAGS   = 26,
    PROP_SAMPLE_POINTS      = 27,
    PROP_LOCK_CONTENT       = 28,
    PROP_GROUP_ITEM         = 29,
    PROP_ITEM_PATH          = 30,
    PROP_GROUP_ITEM_FLAGS   = 31,
    PROP_LOCK_POSITION      = 32,
    PROP_FLOAT_OPACITY      = 33,
    PROP_COLOR_TAG          = 34,
    PROP_COMPOSITE_MODE     = 35,
    PROP_COMPOSITE_SPACE    = 36,
    PROP_BLEND_SPACE        = 37,
    PROP_FLOAT_COLOR        = 38
};

global const char *XCF_PROP_NAMES[] =
{
    "PROP_END",
    "PROP_COLORMAP",
    "PROP_ACTIVE_LAYER",
    "PROP_ACTIVE_CHANNEL",
    "PROP_SELECTION",
    "PROP_FLOATING_SELECTION",
    "PROP_OPACITY",
    "PROP_MODE",
    "PROP_VISIBLE",
    "PROP_LINKED",
    "PROP_LOCK_ALPHA",
    "PROP_APPLY_MASK",
    "PROP_EDIT_MASK",
    "PROP_SHOW_MASK",
    "PROP_SHOW_MASKED",
    "PROP_OFFSETS",
    "PROP_COLOR",
    "PROP_COMPRESSION",
    "PROP_GUIDES",
    "PROP_RESOLUTION",
    "PROP_TATTOO",
    "PROP_PARASITES",
    "PROP_UNIT",
    "PROP_PATHS",
    "PROP_USER_UNIT",
    "PROP_VECTORS",
    "PROP_TEXT_LAYER_FLAGS",
    "PROP_SAMPLE_POINTS",
    "PROP_LOCK_CONTENT",
    "PROP_GROUP_ITEM",
    "PROP_ITEM_PATH",
    "PROP_GROUP_ITEM_FLAGS",
    "PROP_LOCK_POSITION",
    "PROP_FLOAT_OPACITY",
    "PROP_COLOR_TAG",
    "PROP_COMPOSITE_MODE",
    "PROP_COMPOSITE_SPACE",
    "PROP_BLEND_SPACE",
    "PROP_FLOAT_COLOR",
};

#define XCF_UNIT_IN 1
#define XCF_UNIT_MM 2
#define XCF_UNIT_PT 3
#define XCF_UNIT_PC 4

enum GIMP_PRECISION
{
    GIMP_PRECISION_U8_LINEAR     = 100, /*< desc="8-bit linear integer"         >*/
    GIMP_PRECISION_U8_GAMMA      = 150, /*< desc="8-bit gamma integer"          >*/
    GIMP_PRECISION_U16_LINEAR    = 200, /*< desc="16-bit linear integer"        >*/
    GIMP_PRECISION_U16_GAMMA     = 250, /*< desc="16-bit gamma integer"         >*/
    GIMP_PRECISION_U32_LINEAR    = 300, /*< desc="32-bit linear integer"        >*/
    GIMP_PRECISION_U32_GAMMA     = 350, /*< desc="32-bit gamma integer"         >*/
    GIMP_PRECISION_HALF_LINEAR   = 500, /*< desc="16-bit linear floating point" >*/
    GIMP_PRECISION_HALF_GAMMA    = 550, /*< desc="16-bit gamma floating point"  >*/
    GIMP_PRECISION_FLOAT_LINEAR  = 600, /*< desc="32-bit linear floating point" >*/
    GIMP_PRECISION_FLOAT_GAMMA   = 650, /*< desc="32-bit gamma floating point"  >*/
    GIMP_PRECISION_DOUBLE_LINEAR = 700, /*< desc="64-bit linear floating point" >*/
    GIMP_PRECISION_DOUBLE_GAMMA  = 750  /*< desc="64-bit gamma floating point"  >*/
};

enum GIMP_COLOR_TAG
{
    GIMP_COLOR_TAG_BLUE   = 0x01000000,
    GIMP_COLOR_TAG_GREEN  = 0x02000000,
    GIMP_COLOR_TAG_YELLOW = 0x03000000,
    GIMP_COLOR_TAG_ORANGE = 0x04000000,
    GIMP_COLOR_TAG_BROWN  = 0x05000000,
    GIMP_COLOR_TAG_RED    = 0x06000000,
    GIMP_COLOR_TAG_VIOLET = 0x07000000,
    GIMP_COLOR_TAG_GRAY   = 0x08000000,
};

#pragma pack(push, 1)

struct XCFImageHeader
{
    u8 magic_value[9]; // "gimp xcf "
    u8 version[4]; // v0:"file", v1:"v001", v2:"v002", ...
    u8 zero; // 0
    u32 width; // width do canvas 
    u32 height; // height do canvas 
    u32 base_type; // color mode: 0:RGB 1:Grayscale 2:Indexed
    // u32 pointers to layer structures, topmost layer first, zero terminated
    // u32 pointers to channel structures, no particular order, zero terminated
};


struct XCFPropertyRecordHeader
{
    u32 prop_type;
    u32 prop_size; // tamanho dos dados em bytes
    // payload
};

struct XCFLayerHeader
{
    u32 width;
    u32 height;
    u32 color_mode;
};

#pragma pack(pop)

#define ConsumeOffset(stream,version) ((version >= 11) ? U64Swap(*Consume((stream),u64)) : U32Swap(*Consume((stream),u32)))

internal void
PrintBinary (u8 *data, u32 size, u32 stride)
{
    assert(stride <= size)
    for (u32 i = 0; i < size; i += stride)
    {
        for (u32 j = 0; j < stride; ++j)
        {
            u8 byte = data[i+j];
            for (u32 k = 0; k < 8; ++k)
            {
                u8 mask = 0b10000000 >> k;
                printf("%d", (((byte & mask) == mask) ? 1 : 0));
            }
            // printf(" ");
        }
        // printf("\n");
    }
}

internal Level2D
Level2DFromXCF (u8 *xcf_data, u32 size)
{
    Level2D res = {};

    FileStream stream = {};
    stream.data = xcf_data;
    stream.size = size;

    XCFImageHeader *header = Consume(&stream,XCFImageHeader);
    if(header)
    {
        // vars necessarias (export?)
        u32 precision;
        u32 unit_id = 0;
        u8 compression = 0;

        u32 version;
        if(AreEqual(header->version,"file"))
        {
            version = 0;
        }
        else
        {
            version = atoi((const char*)&header->version[1]);
        }
        printf("version: %u\n", version);

        header->width = U32Swap(header->width);
        header->height = U32Swap(header->height);
        printf("width:%u height:%u\n", header->width, header->height);
        res.width = header->width;
        res.height = header->height;
        r32 ratio = ((r32)res.width)/res.height;

        header->base_type = U32Swap(header->base_type);
        printf("color mode: %u\n", header->base_type);

        if(version >= 4)
        {
            precision = U32Swap(*Consume(&stream,u32));
            printf("precision: %u\n", precision);
        }

        while(true)
        {
            XCFPropertyRecordHeader *image_prop = Consume(&stream,XCFPropertyRecordHeader);
            image_prop->prop_type = U32Swap(image_prop->prop_type);
            image_prop->prop_size = U32Swap(image_prop->prop_size);

            char *prop_name;
            if(image_prop->prop_type > (sizeof(XCF_PROP_NAMES)/sizeof(const char *))) prop_name = (char*)"unknown";
            else prop_name = (char*)XCF_PROP_NAMES[image_prop->prop_type];
            
            printf("\t-type:%s(%u) size:%u\n", prop_name,image_prop->prop_type,image_prop->prop_size);
            
            switch(image_prop->prop_type)
            {
                // case PROP_RESOLUTION:
                // {
                //     u8 *data = (u8*)ConsumeSize(&stream, image_prop->prop_size);
                //     u32 w = (u32)((r32*)data)[0];
                //     u32 h = (u32)((r32*)data)[1];
                //     printf("\t\tw:");
                //     PrintBinary(data, 4, 4);
                //     printf(" h:");
                //     PrintBinary((data+4), 4, 4);
                //     printf("\n");
                // } break;

                case PROP_UNIT:
                {
                    unit_id = U32Swap(*Consume(&stream,u32));
                    switch(unit_id)
                    {
                        case XCF_UNIT_IN:  printf("\t\tunit:inches\n");         break;
                        case XCF_UNIT_MM:  printf("\t\tunit:millimiters\n");    break;
                        case XCF_UNIT_PT:  printf("\t\tunit:points\n");         break;
                        case XCF_UNIT_PC:  printf("\t\tunit:picas\n");          break;
                        default:           printf("\t\tunit:unknown(error)\n"); break;
                    }
                } break;
                
                case PROP_PARASITES:
                {
                    u32 base = stream.offset;
                    while((stream.offset - base) < image_prop->prop_size)
                    {
                        u32 name_length = U32Swap(*Consume(&stream,u32));
                        char *name = (char*)ConsumeSize(&stream,name_length);
                        printf("\t\tn:%s", name);
                        u32 flags = U32Swap(*Consume(&stream,u32));
                        u32 length = U32Swap(*Consume(&stream,u32));
                        printf(" f:");
                        PrintBinary((u8*)&flags,4,4);
                        printf(" l:%d\n", length);
                        ConsumeSize(&stream,length);
                    }
                } break;

                case PROP_VECTORS:
                {
                    u32 version = U32Swap(*Consume(&stream, u32));
                    u32 aindex = U32Swap(*Consume(&stream, u32));
                    u32 npaths = U32Swap(*Consume(&stream, u32));
                    printf("\t\tv:%u, aindex:%u, npaths:%u\n", version,aindex,npaths);
                    for (u32 i = 0; i < npaths; ++i)
                    {
                        u32 name_length = U32Swap(*Consume(&stream, u32));
                        char *path_name = (char*)ConsumeSize(&stream, name_length);
                        u32 tattoo = U32Swap(*Consume(&stream, u32));
                        u32 visible = U32Swap(*Consume(&stream, u32));
                        u32 linked = U32Swap(*Consume(&stream, u32));
                        u32 nparasites = U32Swap(*Consume(&stream, u32));
                        u32 nstrokes = U32Swap(*Consume(&stream, u32));
                        printf("\t\t\t[%s] v:%u, nparasites:%u, nstrokes:%u\n", path_name,visible,nparasites,nstrokes);
                        
                        for (u32 j = 0; j < nparasites; ++j)
                        {
                            u32 name_length = U32Swap(*Consume(&stream,u32));
                            char *name = (char*)ConsumeSize(&stream,name_length);
                            printf("\t\t\tn:%s", name);
                            u32 flags = U32Swap(*Consume(&stream,u32));
                            u32 length = U32Swap(*Consume(&stream,u32));
                            printf(" f:");
                            PrintBinary((u8*)&flags,4,4);
                            printf(" l:%d\n", length);
                            ConsumeSize(&stream,length);
                        }

                        for (u32 j = 0; j < nstrokes; ++j)
                        {
                            u32 one = U32Swap(*Consume(&stream, u32)); // bezier stroke
                            u32 closed = U32Swap(*Consume(&stream, u32));
                            u32 nf = U32Swap(*Consume(&stream, u32)); // number of floats per point 2 >= nf <= 6
                            u32 npoints = U32Swap(*Consume(&stream, u32));
                            printf("\t\t\t\tbezier:%u, closed:%u, npoints:%u nf:%u\n", one,closed,npoints,nf);
                            for (u32 k = 0; k < npoints; ++k)
                            {
                                u32 type = U32Swap(*Consume(&stream, u32));
                                r32 x = *Consume(&stream, r32);
                                r32 y = *Consume(&stream, r32);
                                if(nf >= 3)
                                {
                                    r32 pressure = *Consume(&stream, r32);
                                    if(nf >= 4)
                                    {
                                        r32 xtilt = *Consume(&stream, r32);
                                        if(nf >= 5)
                                        {
                                            r32 ytilt = *Consume(&stream, r32);
                                            if(nf == 6)
                                            {
                                                r32 wheel = *Consume(&stream, r32);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // ConsumeSize(&stream, image_prop->prop_size);
                } break;

                case PROP_COMPRESSION:
                {
                    compression = *Consume(&stream, u8);
                    printf("\t\tc:%u\n", compression);
                } break;

                default:
                {
                    ConsumeSize(&stream, image_prop->prop_size);
                } break;
            }
                        
            if(image_prop->prop_type == PROP_END) break;
        }
        
        printf("layer offsets:\n");
        u64 offset;
        do
        {
            offset = ConsumeOffset(&stream,version);

            if(offset > 0)
            {
                printf("\t-offset:%llu\n", offset);
                
                FileStream lstream = stream;
                lstream.offset = offset;

                XCFLayerHeader *layer_header = Consume(&lstream,XCFLayerHeader);
                layer_header->width = U32Swap(layer_header->width);
                layer_header->height = U32Swap(layer_header->height);
                layer_header->color_mode = U32Swap(layer_header->color_mode);
                u32 name_length = U32Swap(*Consume(&lstream,u32));
                char *name = (char*)ConsumeSize(&lstream,name_length);
                printf("\t-[%s] w:%u h:%u cm:%u\n", name,layer_header->width,layer_header->height,layer_header->color_mode);
            
                Level2DPlane *plane = (Level2DPlane*)calloc(1,sizeof(Level2DPlane));
                plane->pixel_width = layer_header->width;
                plane->pixel_height = layer_header->height;
                plane->pixel_data = (u8*)calloc((plane->pixel_width*plane->pixel_height),sizeof(u32));
                u8 *plane_pixel_data = plane->pixel_data;
                for (s32 i = 0; i < (plane->pixel_width*plane->pixel_height); ++i)
                {
                    ((u32*)plane->pixel_data)[i] = 0xff000000;
                }

                printf("\t-layer_props:\n");
                while(true)
                {
                    XCFPropertyRecordHeader *image_prop = Consume(&lstream,XCFPropertyRecordHeader);
                    image_prop->prop_type = U32Swap(image_prop->prop_type);
                    image_prop->prop_size = U32Swap(image_prop->prop_size);

                    char *prop_name;
                    if(image_prop->prop_type > (sizeof(XCF_PROP_NAMES)/sizeof(const char *))) prop_name = (char*)"unknown";
                    else prop_name = (char*)XCF_PROP_NAMES[image_prop->prop_type];
                    printf("\t\t-type:%s(%u) size:%u\n", prop_name,image_prop->prop_type,image_prop->prop_size);
                    
                    switch(image_prop->prop_type)
                    {
                        case PROP_COLOR_TAG:
                        {
                            u32 color = *Consume(&lstream,u32);
                            printf("\t\t\tc:%.8x\n", color);
                            switch(color)
                            {
                                case GIMP_COLOR_TAG_GRAY: 
                                    printf("\t\t\tforeground!\n");
                                    if(res.foreground)
                                    {
                                        plane->next = res.foreground;
                                        res.foreground = plane;
                                    }
                                    else res.foreground = plane;
                                break;
                                case GIMP_COLOR_TAG_BLUE: 
                                    printf("\t\t\tbackground!\n");
                                    if(res.background)
                                    {
                                        plane->next = res.background;
                                        res.background = plane;
                                    }
                                    else res.background = plane; 
                                break;
                            }
                        } break;

                        case PROP_OFFSETS:
                        {
                            s32 xoff = S32Swap(*Consume(&lstream,s32));
                            s32 yoff = S32Swap(*Consume(&lstream,s32));
                            plane->c.x = xoff + (plane->pixel_width*0.5f);
                            plane->c.y = yoff + (plane->pixel_height*0.5f);
                        } break;

                        default:
                            ConsumeSize(&lstream, image_prop->prop_size);
                        break;
                    }


                    if(image_prop->prop_type == PROP_END) break;
                }

                plane->dim.x = (plane->pixel_width*ratio) / res.width;
                plane->dim.y = (plane->pixel_height*1.0f) / res.height;
                plane->c.x = (-0.5f * ratio) + ((plane->c.x * ratio) / res.width);
                plane->c.y = (+0.5f)         - ((plane->c.y * 1.0f) / res.height);

                u64 hierarchy_offset = ConsumeOffset(&lstream,version);
                u64 layer_mask_offset = ConsumeOffset(&lstream,version);
                printf("\tho:%llu lmo:%llu\n", hierarchy_offset,layer_mask_offset);

                FileStream hstream = stream;
                hstream.offset = hierarchy_offset;
                u32 hierarchy_width = U32Swap(*Consume(&hstream,u32));
                u32 hierarchy_height = U32Swap(*Consume(&hstream,u32));
                u32 hierarchy_bpp = U32Swap(*Consume(&hstream,u32));
                printf("\thierarchy: w:%u h:%u bpp:%u\n", hierarchy_width,hierarchy_height,hierarchy_bpp);
                
                u64 levels_offset = ConsumeOffset(&hstream,version);
                hstream.offset = levels_offset;
                u32 levels_width = U32Swap(*Consume(&hstream,u32));
                u32 levels_height = U32Swap(*Consume(&hstream,u32));
                printf("\t\tlevel: w:%u h:%u\n", levels_width,levels_height);

                u32 tiles_x = ceilf(((r32)levels_width)/64.0f);
                u32 tiles_y = ceilf(((r32)levels_height)/64.0f);
                printf("\t\tshould be %u by %u tiles\n", tiles_x,tiles_y);

                u32 current_pixel_offset = 0;    

                u8 *tile_pixel_data = (u8*)calloc((64*64),sizeof(u32));

                FileStream tstream = hstream;
                for (s32 y = 0; y < tiles_y; ++y)
                {   
                    for (s32 x = 0; x < tiles_x; ++x)
                    {
                        tstream.offset = ConsumeOffset(&hstream,version);
                        if(tstream.offset)
                        {
                            u32 tile_width  = ((x==(tiles_x-1))&&(levels_width%64)) ? (levels_width%64) : 64;
                            u32 tile_height = ((y==(tiles_y-1))&&(levels_height%64)) ? (levels_height%64) : 64;
                            u32 tile_pixel_count = tile_width * tile_height;
                            printf("[%.2ux%.2u] ", tile_width,tile_height);

                            for (u32 component = 0; component < hierarchy_bpp; ++component)
                            {
                                u32 pixel_count = 0;
                                while(pixel_count < tile_pixel_count)
                                {
                                    u8 opcode = *Consume(&tstream,u8);
                                    if(opcode <= 126)
                                    { // short run of identical byte values
                                        u8 value = *Consume(&tstream,u8);
                                        for (s32 r = 0; r < (opcode+1); ++r)
                                        {
                                            // u32 index = ((current_pixel_offset+(pixel_count++))*4) + component;
                                            u32 index = ((pixel_count++)*4) + component;
                                            // plane_pixel_data[index] = value;
                                            tile_pixel_data[index] = value;
                                        }
                                    }
                                    else if(opcode == 127)
                                    { // long run of identical byte values
                                        u8 p = *Consume(&tstream,u8);
                                        u8 q = *Consume(&tstream,u8);
                                        u8 value = *Consume(&tstream,u8);
                                        s32 repeat = (p*256) + q;
                                        for (s32 r = 0; r < repeat; ++r)
                                        {
                                            // u32 index = ((current_pixel_offset+(pixel_count++))*4) + component;
                                            u32 index = ((pixel_count++)*4) + component;
                                            // plane_pixel_data[index] = value;
                                            tile_pixel_data[index] = value;
                                        }
                                    }
                                    else if(opcode == 128)
                                    { // long run of different byte values
                                        u8 p = *Consume(&tstream,u8);
                                        u8 q = *Consume(&tstream,u8);
                                        s32 repeat = (p*256) + q;
                                        u8 *values = (u8*)ConsumeSize(&tstream, repeat);
                                        for (s32 r = 0; r < repeat; ++r)
                                        {
                                            // u32 index = ((current_pixel_offset+(pixel_count++))*4) + component;
                                            u32 index = ((pixel_count++)*4) + component;
                                            // plane_pixel_data[index] = values[r];
                                            tile_pixel_data[index] = values[r];
                                        }   
                                    }
                                    else if(opcode >= 129)
                                    { // short run of different byte values
                                        u32 n = (256-opcode);
                                        u8 *values = (u8*)ConsumeSize(&tstream, n);
                                        for (s32 r = 0; r < n; ++r)
                                        {
                                            // u32 index = ((current_pixel_offset+(pixel_count++))*4) + component;
                                            u32 index = ((pixel_count++)*4) + component;
                                            // plane_pixel_data[index] = values[r];
                                            tile_pixel_data[index] = values[r];
                                        }   
                                    }

                                    assert(pixel_count <= tile_pixel_count);
                                }
                            }

                            // current_pixel_offset += tile_pixel_count;

                            for (s32 yy = 0; yy < tile_height; ++yy)
                            {   
                                for (s32 xx = 0; xx < tile_width; ++xx)
                                {
                                    ((u32*)plane_pixel_data)[((x*64)+xx) + (((y*64)+yy)*levels_width)] = ((u32*)tile_pixel_data)[xx+(yy*tile_width)];
                                }
                            }

                        }
                        else printf("dados da imagem incompletos!\n");
                    }
                    printf("\n");
                }
                printf("\n");

                free(tile_pixel_data);

                { // flip
                    u32 *ppd = (u32*)plane_pixel_data;   
                    for (s32 y = 0; y < (levels_height/2); ++y)
                    {   
                        for (s32 x = 0; x < levels_width; ++x)
                        {
                            u32 a = x+(y*levels_width);
                            u32 b = x+((levels_height-1-y)*levels_width);
                            u32 v = ppd[a];
                            ppd[a] = ppd[b];
                            ppd[b] = v;

                            if(hierarchy_bpp < 4)
                            {
                                ppd[a] = 0xff000000 | ppd[a];
                                ppd[b] = 0xff000000 | ppd[b];
                            }
                        }
                    }
                }
            }
            
        } while (offset!=0);

        // Consume(&stream,u32);

        printf("channel offsets:\n");
        do
        {
            offset = ConsumeOffset(&stream,version);
            printf("\t-offset:%llu\n", offset);
        } while (offset!=0);
        
    }

    return res;
}