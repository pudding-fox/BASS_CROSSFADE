#include "../bass/bass.h"

HSTREAM crossfade_mixer_create(const DWORD freq, const DWORD chans, const DWORD flags, const void* const user);

BOOL crossfade_mixer_playing();

BOOL crossfade_mixer_get(HSTREAM* const  handle);

HSTREAM* crossfade_mixer_get_all(DWORD* const count);

BOOL crossfade_mixer_peek(HSTREAM* const handle);

BOOL crossfade_mixer_add(const HSTREAM handle, const BOOL fade_in);

BOOL crossfade_mixer_remove(const HSTREAM handle, const BOOL fade_out);

BOOL crossfade_mixer_next(const BOOL fade_in);