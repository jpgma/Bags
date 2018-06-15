
typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char      u8;
typedef long long   s64;
typedef int         s32;
typedef short       s16;
typedef signed char s8;
typedef float  r32;
typedef double r64;
typedef u32 b32;

#define true  ((b32)1)
#define false ((b32)0)

#define internal static
#define global   static
#define assert(x) {if(!(x)) *((int*)0) = 0;}

#define U8_FROM_U8P(p)    (*(u8 *) (p))
#define S8_FROM_U8P(p)    (*(s8 *) (p))
// #define FIXED_FROM_U8P(p) s32(p)
internal u16 U16_FROM_U8P(u8 *p) { return p[0]*256 + p[1]; }
internal s16 S16_FROM_U8P(u8 *p) { return p[0]*256 + p[1]; }
internal u32 U32_FROM_U8P(u8 *p) { return (p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3]; }
internal s32 S32_FROM_U8P(u8 *p) { return (p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3]; }

#define U16Swap(n) ((((u16)n)>>8) | (((u16)n)<<8))
#define S16Swap(n) ((((s16)n) << 8) | ((((s16)n) >> 8) & 0xFF))

inline u32 
U32Swap(u32 n)
{
    n = ((n << 8) & 0xFF00FF00 ) | ((n >> 8) & 0xFF00FF ); 
    return (n << 16) | (n >> 16);
}

inline s32 
S32Swap(s32 n)
{
    n = ((n << 8) & 0xFF00FF00) | ((n >> 8) & 0xFF00FF ); 
    return (n << 16) | ((n >> 16) & 0xFFFF);
}

inline u64
U64Swap(u64 n)
{
    n = ((n << 8) & 0xFF00FF00FF00FF00ULL ) | ((n >> 8) & 0x00FF00FF00FF00FFULL );
    n = ((n << 16) & 0xFFFF0000FFFF0000ULL ) | ((n >> 16) & 0x0000FFFF0000FFFFULL );
    return (n << 32) | (n >> 32);
}

inline s64 
S64Swap(s64 n)
{
    n = ((n << 8) & 0xFF00FF00FF00FF00ULL ) | ((n >> 8) & 0x00FF00FF00FF00FFULL );
    n = ((n << 16) & 0xFFFF0000FFFF0000ULL ) | ((n >> 16) & 0x0000FFFF0000FFFFULL );
    return (n << 32) | ((n >> 32) & 0xFFFFFFFFULL);
}
