
internal void
InitializeMemoryPool (MemoryPool *memory, u32 size)
{
	memory->base = (u8*)Allocate(size);
	memory->size = size;
	memory->used = 0;

	// for (i32 i = 0; i < size; ++i) memory->base[i] = 0;
}

internal void
FreeMemoryPool (MemoryPool *memory)
{
	Deallocate(memory->base,memory->size);
	memory->base = 0;
	memory->size = 0;
}

internal void*
AllocateFromMemoryPool_ (MemoryPool *memory, u32 size)
{
	void *res = 0;
	if(size > 0)
	{
		assert((memory->used + size) <= memory->size);
		res = memory->base + memory->used;
		memory->used += size;
	}
	return res;
}

internal u8 *
PushMemoryPool(MemoryPool *memory)
{
	u8 *res = memory->base + memory->used;
	return res;
}

internal void
PopMemoryPool(MemoryPool *memory, u8 *pop_place)
{
	if(pop_place && (pop_place > memory->base) && (pop_place < memory->base+memory->size))
	{
		memory->used = (pop_place - memory->base);
	}
}