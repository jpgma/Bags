
#define MAX_LOADED_SOUNDS (1<<5)
struct LoadedSound
{
	u32 sample_count;
	u32 channel_count;
	i16 *samples[2];
};

struct PlayingSound
{
	b32 loop;
	b32 from_point;

	u32 samples_played;
	u32 loaded_id;
	r32 volume[2];

	V2 p;

	PlayingSound *next;
};

struct SoundState
{
	LoadedSound *loaded_sounds;
	u32 loaded_sound_count;

	PlayingSound *first_playing_sound;
	PlayingSound *first_free_sound;

	u32 system_buffer_expected_bytes_per_frame;
	u32 system_buffer_samples_per_second;
	u32 system_buffer_bytes_per_sample;
	u32 system_buffer_channel_count;
	u32 system_buffer_safety_bytes;
	u32 system_buffer_size;
	b32 system_buffer_valid;
	
	u32 system_buffer_sample_index;
	u32 system_buffer_last_write;
	u32 system_buffer_last_play;
	
	u32 buffer_channel_count;
	u32 buffer_sample_count;

	i16 volume;
	i16 *samples;
	r32 *mixing_samples;

	V2 left_ear;
	V2 right_ear;
};
// global SoundState *GLOBAL_SOUNDSTATE_P;

internal void InitSoundState (SoundState *sound_state, System *system);
internal void OutputSound (SoundState *sound_state, MemoryPool *memory, MemoryPool *temp_memory);

internal void PlaySound (SoundState *sound_state, u32 loaded_id, b32 loop, r32 right_volume, r32 left_volume, MemoryPool *memory);
internal void PlaySoundFromPoint (SoundState *sound_state, u32 loaded_id, b32 loop, r32 right_volume, r32 left_volume, V2 p, MemoryPool *memory);
internal u32 debug_LoadEntireSound (SoundState *sound_state, const char *filename, MemoryPool *memory, MemoryPool *temp_memory);
internal void SetHearingPoint (SoundState *sound_state, V2 p);