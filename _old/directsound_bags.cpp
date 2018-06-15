
//TODO: get from hardware
#define BUFFER_SAMPLES_PER_SECOND 48000
#define BUFFER_CHANNEL_COUNT 2
#define BUFFER_DURATION_SECONDS 2

typedef HRESULT WINAPI DIRECTSOUNDCREATE(LPGUID,LPDIRECTSOUND*,LPUNKNOWN);
DIRECTSOUNDCREATE *DirectSoundCreate_;

global LPDIRECTSOUNDBUFFER GLOBAL_SECONDARY_BUFFER;

internal b32
InitDirectSound(System *system, SoundState *sound_state)
{
	b32 res = false;

	// Load lib
	HMODULE dsound_library = LoadLibraryA("dsound.dll");
	if(dsound_library)
	{
		DirectSoundCreate_ = (DIRECTSOUNDCREATE*) GetProcAddress(dsound_library, "DirectSoundCreate");
		// get dsound object
		LPDIRECTSOUND DirectSound;
		if(DirectSoundCreate_ && SUCCEEDED(DirectSoundCreate_(0, &DirectSound, 0)))
		{
			if(SUCCEEDED(DirectSound->SetCooperativeLevel(system->window, DSSCL_PRIORITY)))
			{
				DSBUFFERDESC bufferdesc = {};
			    bufferdesc.dwSize = sizeof(DSBUFFERDESC);
				bufferdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
				LPDIRECTSOUNDBUFFER primary_buffer;
				if(SUCCEEDED(DirectSound->CreateSoundBuffer(&bufferdesc, &primary_buffer, 0)))
				{
					WAVEFORMATEX waveformatex = {};
					waveformatex.wFormatTag = WAVE_FORMAT_PCM;
				    waveformatex.nChannels = sound_state->system_buffer_channel_count;
				    waveformatex.nSamplesPerSec = sound_state->system_buffer_samples_per_second;
				    waveformatex.wBitsPerSample = sizeof(i16) * 8;
				    waveformatex.nBlockAlign = (waveformatex.nChannels*waveformatex.wBitsPerSample)/8;
				    waveformatex.nAvgBytesPerSec = waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;

					if(SUCCEEDED(primary_buffer->SetFormat(&waveformatex)))
					{
					    bufferdesc.dwFlags = 0;
						bufferdesc.lpwfxFormat = &waveformatex;
					    bufferdesc.dwBufferBytes = sound_state->system_buffer_size;
						if(SUCCEEDED(DirectSound->CreateSoundBuffer(&bufferdesc, &GLOBAL_SECONDARY_BUFFER, 0)))
						{
							res = true;
						}
						else
						{
							// Nao criou buffer primario
							assert(0);
						}
					}
					else
					{
						// Nao setou o formato do buffer primario
						assert(0);
					}
				}
				else
				{
					// Nao criou buffer primario
					assert(0);
				}
			}
			else
			{
				// Nao setou coop level
				assert(0);
			}
		}
		else
		{
			// Nao obteve dsound
			assert(0);
		}
	}

	return res;
}

internal void
DirectSoundFillBuffer (SoundState *sound_state, DWORD byte_to_lock, DWORD bytes_to_write)
{
	VOID *region_1 = 0;
	DWORD region_1_size = 0;
	VOID *region_2 = 0;
	DWORD region_2_size = 0;
	if(SUCCEEDED(GLOBAL_SECONDARY_BUFFER->Lock(byte_to_lock, bytes_to_write,
										       &region_1, &region_1_size,
										       &region_2, &region_2_size,
											   0)))
	{
		i16 *src_sample = sound_state->samples;

		r32 hz = 261.83f;
		u32 sb_sample_index = sound_state->system_buffer_sample_index;

		i16 *dest_sample = (i16*)region_1;
		u32 region_1_count = (region_1_size/sound_state->system_buffer_bytes_per_sample);
		for (i32 sample_index = 0; 
			 sample_index < region_1_count; 
			 ++sample_index)
		{
			// r32 t = (2.0f*PI*(r32)sb_sample_index++) / (r32)(sound_state->system_buffer_samples_per_second/hz);
			// i16 sample_value = (i16) (sinf(t)*sound_state->volume);

			*(dest_sample++) = *(src_sample++);
			*(dest_sample++) = *(src_sample++);
		}
		dest_sample = (i16*)region_2;
		u32 region_2_count = (region_2_size/sound_state->system_buffer_bytes_per_sample);
		for (i32 sample_index = 0; 
			 sample_index < region_2_count; 
			 ++sample_index)
		{
			// r32 t = (2.0f*PI*(r32)sb_sample_index++) / (r32)(sound_state->system_buffer_samples_per_second/hz);
			// i16 sample_value = (i16) (sinf(t)*sound_state->volume);

			*(dest_sample++) = *(src_sample++);
			*(dest_sample++) = *(src_sample++);
		}
		
		sound_state->system_buffer_sample_index += region_1_count + region_2_count;

		GLOBAL_SECONDARY_BUFFER->Unlock(region_1,region_1_size, region_2,region_2_size);
	}
}

internal void 
InitSoundState (SoundState *sound_state, System *system, MemoryPool *memory)
{
	sound_state->system_buffer_samples_per_second = BUFFER_SAMPLES_PER_SECOND;
	sound_state->system_buffer_channel_count = BUFFER_CHANNEL_COUNT;
	sound_state->system_buffer_bytes_per_sample = sizeof(i16) * sound_state->system_buffer_channel_count;
	sound_state->system_buffer_size = sound_state->system_buffer_samples_per_second*BUFFER_DURATION_SECONDS*sound_state->system_buffer_bytes_per_sample;
	sound_state->system_buffer_expected_bytes_per_frame = (sound_state->system_buffer_samples_per_second/TARGET_FPS)*sound_state->system_buffer_bytes_per_sample;
	sound_state->system_buffer_safety_bytes = sound_state->system_buffer_expected_bytes_per_frame/2;
	sound_state->system_buffer_sample_index = 0;
	sound_state->left_ear = V2(0.0f,0.0f);
	sound_state->right_ear = V2(0.0f,0.0f);
	
	sound_state->volume = 1 ;
	sound_state->buffer_channel_count = BUFFER_CHANNEL_COUNT;
	u32 system_buffer_sample_count = sound_state->system_buffer_size / sizeof(i16);
	sound_state->samples = AllocateFromMemoryPool(memory, i16, system_buffer_sample_count);
	sound_state->mixing_samples = AllocateFromMemoryPool(memory, r32, system_buffer_sample_count);

	// sound_state->buffer_sample_count = sound_state->system_buffer_samples_per_second / (TARGET_FPS*2); // TODO: average fps?

	// GenerateSineWave (sound_state, (sound_state->system_buffer_size/sound_state->system_buffer_bytes_per_sample));
	ClearSoundBuffer(sound_state);

	if(InitDirectSound(system,sound_state))
	{	
		// DirectSoundFillBuffer(sound_state, 0, sound_state->buffer_sample_count);
		GLOBAL_SECONDARY_BUFFER->Play(0, 0, DSBPLAY_LOOPING);
	}
	else
	{
		assert(0);
	}
}

internal void
GetSoundBufferPosition(SoundState *sound_state, MemoryPool *memory)
{
	DWORD play_cursor;
	DWORD write_cursor;
	if(GLOBAL_SECONDARY_BUFFER->GetCurrentPosition(&play_cursor,&write_cursor) == DS_OK)
	{
		sound_state->system_buffer_last_write = write_cursor;
		sound_state->system_buffer_last_play = play_cursor;
		if(!sound_state->system_buffer_valid)
		{
			u32 bytes_betweeen;
			if(write_cursor < play_cursor)
				bytes_betweeen = (sound_state->system_buffer_size - play_cursor) + write_cursor;
			else
				bytes_betweeen = write_cursor - play_cursor;
			r32 latency = ((r32)bytes_betweeen) / (r32)sound_state->system_buffer_bytes_per_sample;
			sound_state->buffer_sample_count = latency;

			sound_state->system_buffer_sample_index = write_cursor / sound_state->system_buffer_bytes_per_sample;
			sound_state->system_buffer_valid = true;
		}
	}
	else
	{
		sound_state->system_buffer_valid = false;
	}
}

internal void 
OutputSound (SoundState *sound_state, MemoryPool *memory, MemoryPool *temp_memory)
{
	if(sound_state->system_buffer_valid)
	{
		DWORD byte_to_lock = (sound_state->system_buffer_sample_index*sound_state->system_buffer_bytes_per_sample)%sound_state->system_buffer_size;
		
		DWORD safe_write_cursor = sound_state->system_buffer_last_write;
		if(safe_write_cursor < sound_state->system_buffer_last_play)
		{
			safe_write_cursor += sound_state->system_buffer_size;
		}
		safe_write_cursor += sound_state->system_buffer_safety_bytes;
		DWORD expected_bytes_per_frame = sound_state->system_buffer_expected_bytes_per_frame;
		DWORD expected_frame_boundary_bytes = (sound_state->system_buffer_last_play + expected_bytes_per_frame);
		b32 audio_is_low_latency = (safe_write_cursor < expected_frame_boundary_bytes);
		
		DWORD target_cursor = 0;
		if(audio_is_low_latency)
		{
			target_cursor = expected_frame_boundary_bytes + expected_bytes_per_frame;
		}
		else
		{
			target_cursor = (sound_state->system_buffer_last_write + 
							 expected_bytes_per_frame + 
							 sound_state->system_buffer_safety_bytes);
		}
		target_cursor = target_cursor % sound_state->system_buffer_size;
		
		DWORD bytes_to_write;
		if(byte_to_lock > target_cursor)
		{
			bytes_to_write = (sound_state->system_buffer_size - byte_to_lock);
			bytes_to_write += target_cursor;
		}
		else
		{
			bytes_to_write = target_cursor - byte_to_lock;
		}
		sound_state->buffer_sample_count = bytes_to_write / sound_state->system_buffer_bytes_per_sample;

		MixPlayingSounds(sound_state, memory, temp_memory);
		// GenerateSineWave (sound_state, sound_state->buffer_sample_count);

		DirectSoundFillBuffer(sound_state, byte_to_lock, bytes_to_write);

		// r32 size_x = 2.0f;
		// u32 point_count = 10;
		// V3 positions[] = 
		// {
		// 	V3(-(size_x*0.5f),0.0f,0.0f),
		// 	V3((size_x*0.5f),0.0f,0.0f),

		// 	V3((sound_state->system_buffer_last_play*size_x / sound_state->system_buffer_size) - (size_x*0.5f),0.15f,0.0f),
		// 	V3(((sound_state->system_buffer_last_play*size_x / sound_state->system_buffer_size) - (size_x*0.5f)),-0.15f,0.0f),

		// 	V3((sound_state->system_buffer_last_write*size_x / sound_state->system_buffer_size) - (size_x*0.5f),0.1f,0.0f),
		// 	V3(((sound_state->system_buffer_last_write*size_x / sound_state->system_buffer_size) - (size_x*0.5f)),-0.1f,0.0f),

		// 	V3((byte_to_lock*size_x / sound_state->system_buffer_size) - (size_x*0.5f),0.15f,0.0f),
		// 	V3(((byte_to_lock*size_x / sound_state->system_buffer_size) - (size_x*0.5f)),-0.15f,0.0f),

		// 	V3((target_cursor*size_x / sound_state->system_buffer_size) - (size_x*0.5f),0.1f,0.0f),
		// 	V3(((target_cursor*size_x / sound_state->system_buffer_size) - (size_x*0.5f)),-0.1f,0.0f),
		// };
		// V3 colors[] = 
		// {
		// 	V3(1.0f,1.0f,1.0f),
		// 	V3(1.0f,1.0f,1.0f),

		// 	V3(1.0f,0.2f,0.2f),
		// 	V3(1.0f,0.2f,0.2f),

		// 	V3(0.2f,1.0f,0.2f),
		// 	V3(0.2f,1.0f,0.2f),

		// 	V3(0.2f,0.2f,1.0f),
		// 	V3(0.2f,0.2f,1.0f),

		// 	V3(1.0f,1.0f,1.0f),
		// 	V3(1.0f,1.0f,1.0f),
		// };

		// RenderSolidImmediate (GLOBAL_RENDERERSTATE_P, GLOBAL_RENDERERSTATE_P->gui_projection_matrix, 
		// 						1.0f, positions, colors, point_count, 0, 0, LINES);
	}
}