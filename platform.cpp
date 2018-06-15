//////////////////////
/// Memory

internal void
InitializeStaticMemory(StaticMemory *memory, u32 size)
{
	if(memory && size)
	{
		memory->used = 0;
		memory->size = size;
		memory->base = (u8*)PlatformAllocateMemory(size);
	}
}

internal u8 *
AllocateStaticMemory_(StaticMemory *memory, u32 size)
{
	assert((memory->size - memory->used) >= size);

	u8 *res = 0;
	if(memory && size)
	{
		if(memory->base)
		{
			res = memory->base + memory->used;
			memory->used += size;
		}
	}
	return res;
}

internal void *
ConsumeSize(FileStream *stream, u32 size)
{
    void *res = 0;
    if(stream->data && ((stream->offset + size) <= stream->size))
    {
        res = (void*)(stream->data + stream->offset);
        stream->offset += size;
    }
    return res;
};

internal void
FreeStaticMemory(StaticMemory *memory)
{
	if(memory)
	{
		if(memory->base)
			PlatformDeallocateMemory(memory->base,memory->size);
		memory->base = 0;
		memory->used = 0;
		memory->size = 0;
	}
}

//////////////////////
/// Input

internal void 
WatchKey (Input *input, StaticMemory *memory, u16 keycode)
{
	WatchedKey *key = AllocateStaticMemory(memory,1,WatchedKey);
	if(key)
	{
		key->keycode = keycode;
		key->next = input->first_watched_key;
		input->first_watched_key = key;
	}
}

internal u16 
InternalToSystemKeyCode (u16 internal_keycode)
{
	u16 res = system_keycodes[internal_keycode];
	return res;
}

internal u16 
SystemToInternalKeyCode (u16 system_keycode)
{
	u16 res = 0;
	u32 count = sizeof(system_keycodes)/sizeof(u16);
	for (u32 i = 0; i < count; ++i)
	{
		if(system_keycodes[i] == system_keycode) 
		{
			res = i;
			break;
		}
	}
	return res;
}

internal WatchedKey
GetWatchedKey (Input *input, u16 internal_keycode)
{
	WatchedKey res = {};

	if(input->first_watched_key)
	{
		WatchedKey *key = input->first_watched_key;
		while(key)
		{
			if(key->keycode == internal_keycode)
			{
				res = *key;
				break;
			}
			key = key->next;
		}
	}

	return res;
}

inline b32 
IsKeyDown(Input *input, u16 internal_keycode)
{
	b32 res = false;

	WatchedKey key = GetWatchedKey(input, internal_keycode);
	if(key.keycode == internal_keycode) 
		res = ((key.state & KEY_STATE_DOWN) == KEY_STATE_DOWN) ? true : false;

	return res;
}

inline b32 
WasKeyJustPressed(Input *input, u16 internal_keycode)
{
	b32 res = false;

	WatchedKey key = GetWatchedKey(input, internal_keycode);
	if(key.keycode == internal_keycode) 
		res = (((key.state & KEY_STATE_DOWN) == KEY_STATE_DOWN) && ((key.state & KEY_STATE_CHANGED) == KEY_STATE_CHANGED)) ? true : false;

	return res;
}

inline b32 
WasKeyJustReleased(Input *input, u16 internal_keycode)
{
	b32 res = false;

	WatchedKey key = GetWatchedKey(input, internal_keycode);
	if(key.keycode == internal_keycode) 
		res = (((key.state & KEY_STATE_DOWN) == 0) && ((key.state & KEY_STATE_CHANGED) == KEY_STATE_CHANGED)) ? true : false;

	return res;
}
