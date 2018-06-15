#define KILOBYTES(n) ((n)*1024)
#define MEGABYTES(n) (KILOBYTES(n)*1024)
#define GIGABYTES(n) (MEGABYTES(n)*1024)

struct MemoryPool
{
	u32 size;
	u32 used;
	u8 *base;
};


struct AllocationHeader
{
	u16 flags;
	u32 used;
	AllocationHeader *next;
};
struct FlexibleMemoryBlock
{
	AllocationHeader *first_allocheader;
	AllocationHeader *first_free_allocheader;
};

internal void InitializeMemoryPool (MemoryPool *memory, u32 size);
internal void FreeMemoryPool (MemoryPool *memory);

internal void *AllocateFromMemoryPool_ (MemoryPool *memory, u32 size);
#define AllocateFromMemoryPool(memory, type, count) ((type*) AllocateFromMemoryPool_((memory), (sizeof(type) * (count))))

internal u8 *PushMemoryPool(MemoryPool *memory);
internal void PopMemoryPool(MemoryPool *memory, u8 *pop_place);