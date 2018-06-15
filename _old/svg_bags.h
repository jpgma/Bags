
#define SVG_MAX_NESTING 100

#define SVG_VTYPE_ABS_MOVETO 'M'
#define SVG_VTYPE_REL_MOVETO 'm'
#define SVG_VTYPE_ABS_LINETO 'L'
#define SVG_VTYPE_REL_LINETO 'l'
#define SVG_VTYPE_ABS_HLINETO 'H'
#define SVG_VTYPE_REL_HLINETO 'h'
#define SVG_VTYPE_ABS_VLINETO 'V'
#define SVG_VTYPE_REL_VLINETO 'v'
#define SVG_VTYPE_ABS_CCURVETO 'C'
#define SVG_VTYPE_REL_CCURVETO 'c'
#define SVG_VTYPE_ABS_SCURVETO 'S'
#define SVG_VTYPE_REL_SCURVETO 's'
#define SVG_VTYPE_ABS_QCURVETO 'Q'
#define SVG_VTYPE_REL_QCURVETO 'q'
#define SVG_VTYPE_ABS_TCURVETO 'T'
#define SVG_VTYPE_REL_TCURVETO 't'
#define SVG_VTYPE_ABS_ARC 'A'
#define SVG_VTYPE_REL_ARC 'a'
#define SVG_VTYPE_ABS_CLOSEPATH 'Z'
#define SVG_VTYPE_REL_CLOSEPATH 'z'

struct VECImage;

inline char *SVG_GetNextTag (char *text);
internal VECImage SVG_GetVECImage(char *svg_text, r32 z_diff, MemoryPool *memory);
// internal u32 *SVG_GetGroupCounts (const char *filename, MemoryPool *memory, u32 *group_count);