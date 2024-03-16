#include "../bass/bass.h"

BOOL crossfade_queue_create();

BOOL crossfade_queue_contains(const HSTREAM handle);

BOOL crossfade_queue_enqueue(const HSTREAM handle);

BOOL crossfade_queue_dequeue(HSTREAM* const handle);

BOOL crossfade_queue_push(const HSTREAM handle);

BOOL crossfade_queue_peek(HSTREAM* const handle);

BOOL crossfade_queue_is_empty(BOOL* const empty);

BOOL crossfade_queue_count(DWORD* const length);

HSTREAM* crossfade_queue_get_all(DWORD* const length);

BOOL crossfade_queue_remove(const HSTREAM handle);

BOOL crossfade_queue_clear();

BOOL crossfade_queue_free();