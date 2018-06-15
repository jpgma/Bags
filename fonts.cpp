
//////////////////////
/// Fonts

struct Gylph
{
    
};

//////////////////////
/// Opentype

#pragma pack(push, 1)

struct OpentypeOffsetTable
{
    u32 sfntVersion;   // 0x00010000 or 0x4F54544F ('OTTO').
    u16 numTables;     // Number of tables.
    u16 searchRange;   // (Maximum power of 2 <= numTables) x 16.
    u16 entrySelector; // Log2(maximum power of 2 <= numTables).
    u16 rangeShift;    // NumTables x 16-searchRange.
};

struct OpentypeTableRecord
{
    u32 tag;      // 4 -byte identifier.
    u32 checkSum; // CheckSum for this table.
    u32 offset;   // Offset from beginning of TrueType font file.
    u32 length;   // Length of this table.
};

struct TruetypeGlyphHeader
{
    s16 numberOfContours; //  >=0: single glyph, <0: composite glyph(-1 should be used)
    s16 xMin; // Minimum x for coordinate data.
    s16 yMin; // Minimum y for coordinate data.
    s16 xMax; // Maximum x for coordinate data.
    s16 yMax; // Maximum y for coordinate data.
};

struct OpentypeMAXP
{
    u32 version; // 0x00005000 for version 0.5, 0x00010000 for version 1.0.
    u16 numGlyphs; // The number of glyphs in the font.
};

struct OpentypeHEAD
{
    u16 majorVersion; //    Major version number of the font header table — set to 1.
    u16 minorVersion; //    Minor version number of the font header table — set to 0.
    u32 fontRevision; //    Set by font manufacturer.
    u32 checkSumAdjustment; //  To compute: set it to 0, sum the entire font as uint32, then store 0xB1B0AFBA - sum. If the font is used as a component in a font collection file, the value of this field will be invalidated by changes to the file structure and font table directory, and must be ignored.
    u32 magicNumber; // Set to 0x5F0F3CF5.
    u16 flags; //   Bit 0: Baseline for font at y=0;
    u16 unitsPerEm; //  Set to a value from 16 to 16384. Any value in this range is valid. In fonts that have TrueType outlines, a power of 2 is recommended as this allows performance optimizations in some rasterizers.
    u64 created; // Number of seconds since 12:00 midnight that started January 1st 1904 in GMT/UTC time zone. 64-bit integer
    u64 modified; //    Number of seconds since 12:00 midnight that started January 1st 1904 in GMT/UTC time zone. 64-bit integer
    s16 xMin; //    For all glyph bounding boxes.
    s16 yMin; //    For all glyph bounding boxes.
    s16 xMax; //    For all glyph bounding boxes.
    s16 yMax; //    For all glyph bounding boxes.
    u16 macStyle; //
    u16 lowestRecPPEM; //   Smallest readable size in pixels.
    s16 fontDirectionHint; //
    s16 indexToLocFormat; //    0 for short offsets (Offset16), 1 for long (Offset32).
    s16 glyphDataFormat; // 0 for current format.
};

#pragma pack(pop)

struct OpentypeParser
{
    OpentypeOffsetTable *offset_table;
    OpentypeTableRecord *table_records;

};

internal u32
GetTableOffset (OpentypeParser *parser, u32 tag)
{
    u32 res = 0;

    tag = U32Swap(tag);
    u16 numTables = parser->offset_table->numTables;
    for (u16 i = 0; i < numTables; ++i)
    {
        if(parser->table_records[i].tag == tag)
            res = U32Swap(parser->table_records[i].offset);
    }

    return res;
}

internal b32
ParseOpentypeFont(const char *filename)
{
    b32 res = true;
    
    RawFileContents rfc = PlatformGetRawFileContents(filename, 0);

    if(rfc.contents)
    {
        printf("-parsing \"%s\" -\n", filename);
        FileStream stream = {};
        stream.data = rfc.contents;
        stream.size = rfc.size;

        OpentypeParser parser = {};
        parser.offset_table = Consume(&stream,OpentypeOffsetTable);
        parser.table_records = Consume(&stream,OpentypeTableRecord);
        
        parser.offset_table->numTables = U16Swap(parser.offset_table->numTables);

        printf("--version: %.4s(%.4x)\n", 
               (char*)&parser.offset_table->sfntVersion,parser.offset_table->sfntVersion);

        OpentypeMAXP *maxp = (OpentypeMAXP*)(stream.data + GetTableOffset(&parser, 'maxp'));
        maxp->version = U32Swap(maxp->version);
        maxp->numGlyphs = U16Swap(maxp->numGlyphs);
        printf("--maxp v:%.8x num_glyphs:%u\n", maxp->version, maxp->numGlyphs);

        OpentypeHEAD *head = (OpentypeHEAD*)(stream.data + GetTableOffset(&parser, 'head'));
        printf("--head [%.8x] %d\n", U32Swap(head->magicNumber), S16Swap(head->indexToLocFormat));

        u8 *loca = stream.data + GetTableOffset(&parser, 'loca');

        if(parser.offset_table->sfntVersion == 'OTTO') // cff v1 or v2
        {
            u32 cff_offset = GetTableOffset(&parser, 'CFF '); // cff v1
            if(cff_offset)
            {
                printf("--CFF 1.0\n");
            }
        }
        else if(parser.offset_table->sfntVersion == 0x0100) // truetype outlines
        {
            u32 glyf_offset = GetTableOffset(&parser, 'glyf');
            if(glyf_offset)
            {
                printf("--glyf\n");

                for (s32 i = 0; i < maxp->numGlyphs; ++i)
                {
                    u32 glyph_offset = 0;
                    if(head->indexToLocFormat) glyph_offset = U32Swap(((u32*)loca)[i]);
                    else glyph_offset = U16Swap(((u16*)loca)[i]) * 2;

                    stream.offset = glyph_offset;
                    TruetypeGlyphHeader *glyph_header = Consume(&stream,TruetypeGlyphHeader);
                    printf("---nc:%d \n", (glyph_header->numberOfContours));
                }
            }
        }
        else
        {
            res = false;
        }
    }

    return res;
}