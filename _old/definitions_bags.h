#include <stdint.h>

//
// Definicoes Basicas
//

#define persist static
#define global static
#define internal static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float r32;
typedef double r64;

typedef size_t sizet;

#define I32_MIN ((i32)0x80000000)
#define I32_MAX ((i32)0x7FFFFFFF)

// BIG ENDIAN
#define U8_FROM_U8P(p)    (*(u8 *) (p))
#define I8_FROM_U8P(p)    (*(i8 *) (p))
#define FIXED_FROM_U8P(p) I32(p)
internal u16 U16_FROM_U8P(u8 *p) { return p[0]*256 + p[1]; }
internal i16 I16_FROM_U8P(u8 *p) { return p[0]*256 + p[1]; }
internal u32 U32_FROM_U8P(u8 *p) { return (p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3]; }
internal i32 I32_FROM_U8P(u8 *p) { return (p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3]; }

#define U16Swap(n) ((((u16)n)>>8) | (((u16)n)<<8))
#define I16Swap(n) ((((i16)n) << 8) | ((((i16)n) >> 8) & 0xFF))
inline u32 
U32Swap(u32 n)
{
    n = ((n << 8) & 0xFF00FF00 ) | ((n >> 8) & 0xFF00FF ); 
    return (n << 16) | (n >> 16);
}
inline i32 
I32Swap(i32 n)
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
inline i64 
I64Swap(i64 n)
{
    n = ((n << 8) & 0xFF00FF00FF00FF00ULL ) | ((n >> 8) & 0x00FF00FF00FF00FFULL );
    n = ((n << 16) & 0xFFFF0000FFFF0000ULL ) | ((n >> 16) & 0x0000FFFF0000FFFFULL );
    return (n << 32) | ((n >> 32) & 0xFFFFFFFFULL);
}

inline r32
FixedToR32(u32 n)
{
	n = U32_FROM_U8P((u8*)&n);
	r32 integer = (r32)((u16)(n >> 16));
	r32 decimal = (r32)(((u16)((n << 16) >> 16))) / 100000.0f;
	return integer + decimal;
}

typedef i32 b32;
#define true 1
#define false 0

#define InvalidPath (*(int *)0 = 0)
//#define InvalidDefault default: {InvalidPath;} break
#define assert(x) {if(!(x)){InvalidPath;}}

#define PACK_TAG(a,b,c,d) (u32)((((u32)((u8)d))<<24) | ((((u32)((u8)c))<<24)>>8) | ((((u32)((u8)b))<<24)>>16) | ((((u32)((u8)a))<<24)>>24))
#define UNPACK_TAG(tag) \
{\
    (char)((((u32)tag) << 24) >> 24),\
    (char)((((u32)tag) << 16) >> 24),\
    (char)((((u32)tag) << 8) >> 24),\
    (char)(((u32)tag) >> 24),\
    '\0'\
}

#define FORWARD_LINKED_LIST(name,type,type_name) \
struct name \
{\
    type *type_name;\
    name *next;\
}