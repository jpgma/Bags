internal u32
debug_LoadEntireSound (SoundState *sound_state, const char *filename, 
							MemoryPool *memory, MemoryPool *temp_memory)
{
	assert(sound_state->loaded_sound_count <= MAX_LOADED_SOUNDS);
	u32 res = sound_state->loaded_sound_count;

	if(!sound_state->loaded_sounds) 
	{
		sound_state->loaded_sounds = AllocateFromMemoryPool(memory, LoadedSound, MAX_LOADED_SOUNDS);
		sound_state->loaded_sound_count = 0;
	}

	sound_state->loaded_sounds[sound_state->loaded_sound_count++] = LoadWAV (filename, memory, temp_memory);

	return res;
}

internal void 
PlaySoundFromPoint (SoundState *sound_state, u32 loaded_id, b32 loop, r32 right_volume, r32 left_volume, V2 p, MemoryPool *memory)
{
	PlayingSound *playing_sound;
	if(sound_state->first_free_sound)
	{
		playing_sound = sound_state->first_free_sound;
		sound_state->first_free_sound = sound_state->first_free_sound->next;
	}
	else
	{
		playing_sound = AllocateFromMemoryPool(memory, PlayingSound, 1);
	}
	
	playing_sound->p = p;
	playing_sound->loop = loop;
	playing_sound->from_point = true;
	playing_sound->samples_played = 0;
	playing_sound->loaded_id = loaded_id;
	playing_sound->volume[0] = left_volume;
	playing_sound->volume[1] = right_volume;

	playing_sound->next = sound_state->first_playing_sound;
	sound_state->first_playing_sound = playing_sound;
}

internal void 
PlaySound (SoundState *sound_state, u32 loaded_id, b32 loop, r32 right_volume, r32 left_volume, MemoryPool *memory)
{
	PlayingSound *playing_sound;
	if(sound_state->first_free_sound)
	{
		playing_sound = sound_state->first_free_sound;
		sound_state->first_free_sound = sound_state->first_free_sound->next;
	}
	else
	{
		playing_sound = AllocateFromMemoryPool(memory, PlayingSound, 1);
	}
	
	playing_sound->loop = loop;
	playing_sound->from_point = false;
	playing_sound->samples_played = 0;
	playing_sound->loaded_id = loaded_id;
	playing_sound->volume[0] = left_volume;
	playing_sound->volume[1] = right_volume;

	playing_sound->next = sound_state->first_playing_sound;
	sound_state->first_playing_sound = playing_sound;
}

internal void
SetHearingPoint (SoundState *sound_state, V2 p)
{
	r32 distance = 0.5f;
	sound_state->left_ear  = p - V2((distance*0.5f),0.0f);
	sound_state->right_ear = p + V2((distance*0.5f),0.0f);
}

internal void
MixPlayingSounds (SoundState *sound_state, MemoryPool *memory, MemoryPool *temp_memory)
{
	r32 *src_sample = sound_state->mixing_samples;
	for (u32 sample_index = 0;
		 sample_index < sound_state->buffer_sample_count; 
		 ++sample_index)
	{
		*(src_sample++) = 0.0f;
		*(src_sample++) = 0.0f;
	}

	PlayingSound *last_sound = 0;
	PlayingSound *sound = sound_state->first_playing_sound;
	while(sound)
	{
		LoadedSound loaded_sound = sound_state->loaded_sounds[sound->loaded_id];

		u32 samples_to_mix = sound_state->buffer_sample_count;
		u32 samples_remaining = loaded_sound.sample_count - sound->samples_played;
		if(samples_remaining < samples_to_mix)
			samples_to_mix = samples_remaining;

		r32 falloff = 1.0f;
		r32 left_volume;
		r32 right_volume;
		if(sound->from_point)
		{
			V2 dp = sound_state->left_ear - sound->p;
			r32 mag_sq = Magnitude(dp)+1.0f;
			// mag_sq *= mag_sq;
			if(dp.x >= 0.0f)
				left_volume = sound->volume[0] / mag_sq;
			else
				left_volume = sound->volume[0] / (mag_sq*falloff);
			
			dp = sound_state->right_ear - sound->p;
			mag_sq = Magnitude(dp)+1.0f;
			// mag_sq *= mag_sq;
			if(dp.x <= 0.0f)
				right_volume = sound->volume[1] / mag_sq;
			else
				right_volume = sound->volume[1] / (mag_sq*falloff);

			// char txt[256];
			// sprintf(txt, "l: %.4f r:%.4f\n", left_volume,right_volume);
			// RenderText(GLOBAL_RENDERERSTATE_P, &GLOBAL_RENDERERSTATE_P->debug_font, 
			// 			GLOBAL_RENDERERSTATE_P->gui_projection_matrix, 
			// 			txt, 
			// 			512, 0.2f, V2(0.0,0.0f), V3(1.0f,1.0f,1.0f), 1.0f, 
			// 			memory, temp_memory);
		}
		else
		{
			left_volume = sound->volume[0];
			right_volume = sound->volume[1];
		}

		r32 *mixing_sample = sound_state->mixing_samples;
		for (u32 sample_index = sound->samples_played;
			 sample_index < (sound->samples_played + samples_to_mix); 
			 ++sample_index)
		{
			r32 sample_value = loaded_sound.samples[0][sample_index];
			*(mixing_sample++) += ((sample_value) * left_volume);
			if(loaded_sound.channel_count == 2)
				sample_value = loaded_sound.samples[1][sample_index];
			*(mixing_sample++) += ((sample_value) * right_volume);
		}
		sound->samples_played += samples_to_mix;
		
		// sound = 0;
		PlayingSound *next_sound = sound->next;

		if(sound->samples_played == loaded_sound.sample_count)
		{
			if(sound->loop) 
			{
				sound->samples_played = 0;
			}
			else
			{
				if(sound == sound_state->first_playing_sound)
					sound_state->first_playing_sound = next_sound;
				else
					last_sound->next = sound->next;
				sound->next = sound_state->first_free_sound;
				sound_state->first_free_sound = sound;
			}
		}

		last_sound = sound;
		sound = next_sound;
	}

	// r32 p/ i16
	src_sample = sound_state->mixing_samples;
	i16 *dest_sample = sound_state->samples;
	for (u32 sample_index = 0; 
		 sample_index < sound_state->buffer_sample_count; 
		 ++sample_index)
	{
		*(dest_sample++) = (i16) (*(src_sample++) + 0.5f) * sound_state->volume;
		*(dest_sample++) = (i16) (*(src_sample++) + 0.5f) * sound_state->volume;
	}
}

internal void
RenderPlayingSound(SoundState *sound_state, RendererState *renderer_state, 
					PlayingSound *playing_sound, V3 p, r32 size_x, r32 size_y, MemoryPool *memory)
{
	LoadedSound loaded_sound = sound_state->loaded_sounds[playing_sound->loaded_id];

	u32 samples_played = playing_sound->samples_played;
	u32 sample_count = sound_state->buffer_sample_count*2;
	r32 dx = (size_x/sample_count);
	r32 px = -(size_x*0.5f);

	u8 *pop = PushMemoryPool(memory);
	V3 *positions_left = AllocateFromMemoryPool(memory, V3, sample_count);
	V3 *colors_left = AllocateFromMemoryPool(memory, V3, sample_count);
	V3 *positions_right = AllocateFromMemoryPool(memory, V3, sample_count);
	V3 *colors_right = AllocateFromMemoryPool(memory, V3, sample_count);

	i16 *left_sample = loaded_sound.samples[0]  + samples_played - (u32)(sample_count*0.5f);
	i16 *right_sample = loaded_sound.samples[1] + samples_played - (u32)(sample_count*0.5f);
	for (u32 sample_index = 0; 
		 sample_index < sample_count; 
		 ++sample_index)
	{
		V3 color;
		if(sample_index == (sample_count/2))
			color = V3(1.0f,0.2f,0.2f);
		else
			color = V3(1.0f,1.0f,1.0f);

		r32 py = (size_y / *(left_sample++)) + (size_y*0.5f);
		positions_left[sample_index] = V3(px, py, 0.0f);
		colors_left[sample_index] = color;

		if(loaded_sound.channel_count > 1)
			py = (size_y / *(right_sample++)) - (size_y*0.5f);
		else
			py = 0;
		positions_right[sample_index] = V3(px, py, 0.0f);
		colors_right[sample_index] = color;

		px += dx;
	}

	Mat4 mvp = {};
	ApplyTransforms(&mvp, p, V3(1.0f,1.0f,1.0f), QUAT_ZERO);
	mvp = mvp * renderer_state->gui_projection_matrix;

	RenderSolidImmediate (renderer_state, mvp, 1.0f, positions_left, colors_left, sample_count, 0, 0, LINE_STRIP);
	RenderSolidImmediate (renderer_state, mvp, 1.0f, positions_right, colors_right, sample_count, 0, 0, LINE_STRIP);
	
	PopMemoryPool(memory, pop);
}

internal void
GenerateSineWave (SoundState *sound_state, u32 sample_count_to_output)
{
	if(sound_state->samples)
	{
		r32 hz = 256.0f;

		u32 sb_sample_index = sound_state->system_buffer_sample_index;

		i16 *dest_sample = sound_state->samples;
		for (u32 sample_index = 0; 
			 sample_index < sample_count_to_output; 
			 ++sample_index)
		{
			r32 t = (2.0f*PI*(r32)sb_sample_index++) / (r32)(sound_state->system_buffer_samples_per_second/hz);
			i16 sample_value = (i16) (sinf(t)*3000);
			*(dest_sample++) = sample_value;
			*(dest_sample++) = sample_value;
		}
	}
}

internal void
ClearSoundBuffer (SoundState *sound_state)
{
	if(sound_state->samples)
	{
		i16 *dest_sample = sound_state->samples;
		for (u32 sample_index = 0; 
			 sample_index < sound_state->buffer_sample_count; 
			 ++sample_index)
		{
			*(dest_sample++) = 0;
			*(dest_sample++) = 0;
		}
	}
}