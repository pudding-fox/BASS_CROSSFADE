#include "../bass/bass.h"

#define ENVELOPE_POINTS 20

BOOL crossfade_curve_populate(const DWORD type, const float min, const float max, float curve[ENVELOPE_POINTS]);

BOOL crossfade_envelope_apply_in(const HSTREAM handle);

BOOL crossfade_envelope_apply_out(const HSTREAM handle, const BOOL remove);