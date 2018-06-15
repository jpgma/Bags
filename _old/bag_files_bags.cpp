global const u32 TAG_SGW  = PACK_TAG('s','g','w',' ');
global const u32 TAG_FONT = PACK_TAG('f','o','n','t');
struct BagHeader
{
    u32 tag;
    u32 count;
    u64 beginning;
};

#define BAG_POINTER(header,data,type,pointer) (type*) (pointer ? ( (data) + (u64)(((u64)pointer)-((header)->beginning)) ) : 0);