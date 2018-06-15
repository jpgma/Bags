
#define MPEG_ALLIGN_MASK     ((u32)0b11111111111000000000000000000000)
#define MPEG_VERSION_MASK    ((u32)0b00000000000110000000000000000000)
#define MPEG_LAYER_MASK      ((u32)0b00000000000001100000000000000000)
#define MPEG_PROTECTION_MASK ((u32)0b00000000000000010000000000000000)
#define MPEG_BITRATE_MASK    ((u32)0b00000000000000001111000000000000)
#define MPEG_SAMPLERATE_MASK ((u32)0b00000000000000000000110000000000)
#define MPEG_PADDING_MASK    ((u32)0b00000000000000000000001000000000)
#define MPEG_CHANNEL_MASK    ((u32)0b00000000000000000000000011000000)
#define MPEG_MODEEX_MASK     ((u32)0b00000000000000000000000000110000)
#define MPEG_COPYRIGHT_MASK  ((u32)0b00000000000000000000000000001000)
#define MPEG_ORIGINAL_MASK   ((u32)0b00000000000000000000000000000100)
#define MPEG_EMPHASIS_MASK   ((u32)0b00000000000000000000000000000011)

enum MPEG_VERSION
{
    MPEG_VERSION_2_5 = 0b00000000000000000000000000000000,  
    MPEG_VERSION_RES = 0b00000000000010000000000000000000,  
    MPEG_VERSION_2   = 0b00000000000100000000000000000000,  
    MPEG_VERSION_1   = 0b00000000000110000000000000000000,  
};

enum MPEG_LAYER
{
    MPEG_LAYER_RES = 0b00000000000000000000000000000000,  
    MPEG_LAYER_3   = 0b00000000000000100000000000000000,  
    MPEG_LAYER_2   = 0b00000000000001000000000000000000,  
    MPEG_LAYER_1   = 0b00000000000001100000000000000000,  
};

#define MPEG_BITRATE_INDEX(header) (((((u32)(header))&MPEG_BITRATE_MASK)>>12)&0b00000000000000000000000000001111)

global u32 MPEG_BITRATE_V1L1[] = 
{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0 };

global u32 MPEG_BITRATE_V1L2[] = 
{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0 };

global u32 MPEG_BITRATE_V1L3[] = 
{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 };

global u32 MPEG_BITRATE_V2L1[] = 
{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0 };

global u32 MPEG_BITRATE_V2L2L3[] = 
{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 };

#define MPEG_SAMPLERATE_INDEX(header) (((((u32)(header))&MPEG_SAMPLERATE_MASK)>>10)&0b00000000000000000000000000000011)

global u32 MPEG_SAMPLERATE_V1[]   = { 44100, 48000, 32000, 0 };
global u32 MPEG_SAMPLERATE_V2[]   = { 22050, 24000, 16000, 0 };
global u32 MPEG_SAMPLERATE_V2_5[] = { 11025, 12000,  8000, 0 };
/*
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#include "..\\code\\projects\\bags\\definitions.h"
#include "..\\code\\projects\\bags\\mpeg.h"

char *
GetTextFileContents (const char *filename)
{
    char *res = 0;

    FILE *file = fopen(filename,"r");
    if(file)
    {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        res = (char*)calloc(1,file_size+1);
        fread(res, file_size, 1, file);
        fclose(file);
    }

    return res;
}

u8 *
GetFileContents (const char *filename, u32 *size)
{
    u8 *res = 0;

    FILE *file = fopen(filename,"r+");
    if(file)
    {
        fseek(file, 0, SEEK_END);
        u32 file_size = ftell(file);
        if(size) *size = file_size;
        fseek(file, 0, SEEK_SET);
        res = (u8*)calloc(1,file_size);
        fread(res, file_size, 1, file);
        fclose(file);
    }

    return res;
}

void
WriteTextFile (const char *filename, char *text)
{
    int file_size = 0;
    { // contar tamanho do texto
        char *c = text;
        while(*(c++)) file_size++;
        file_size += 1;
    }

    FILE *file = fopen(filename,"w");
    fwrite ((const void *)text, 1, file_size, file);
}

void
PrintBinary (u8 *data, u32 size, u32 rows)
{
    for (int i = 0; i < size; i += rows)
    {
        printf("\n");
        for (int j = 0; j < rows; ++j)
        {
            u8 byte = data[i+j];
            for (int k = 0; k < 8; ++k)
            {
                u8 mask = 0b10000000 >> k;
                s32 bit = (((byte & mask) == mask) ? 1 : 0);
                printf("%d", bit);
            }
            printf(" ");
        }
    }
    printf("\n");
}

void main()
{
    const char *filename = "light_sword_draw.mp3";
    u32 mp3_size = 0;
    u8 *mp3_data = GetFileContents(filename, &mp3_size);

    if(mp3_data)
    {
        u32 header = *((u32*)mp3_data) & MPEG_BITRATE_MASK;
        PrintBinary ((u8*)&header, 4, 4);

        u32 bitrate = 0;
        u32 samplerate = 0;
        u32 index = MPEG_BITRATE_INDEX(header);

        u32 version_layer = header & (MPEG_VERSION_MASK|MPEG_LAYER_MASK);
        if(version_layer == (MPEG_VERSION_1|MPEG_LAYER_1))
        {
            printf("l1v1\n");
            bitrate = MPEG_BITRATE_V1L1[index];
        }
        else if(version_layer == (MPEG_VERSION_1|MPEG_LAYER_2))
        {
            printf("l1v1\n");
            bitrate = MPEG_BITRATE_V1L2[index];
        }
        else if(version_layer == (MPEG_VERSION_1|MPEG_LAYER_3))
        {
            printf("l1v3\n");
            bitrate = MPEG_BITRATE_V1L3[index];
        }

        printf("bitrate:%u samplerate:%u (%u)\n", bitrate, samplerate, index);

        PrintBinary (mp3_data, mp3_size, 4);
    }
}
*/