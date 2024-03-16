#ifdef _DEBUG
#include <stdio.h>
#endif

#include "../bass/bassmix.h"
#include "bass_crossfade.h"
#include "crossfade_config.h"
#include "crossfade_envelope.h"
#include "crossfade_mixer.h"
#include "crossfade_queue.h"
#include "crossfade_syncs.h"

//2.4.0.0
#define BASS_VERSION 0x02040000

//I have no idea how to prevent linking against this routine in msvcrt.
//It doesn't exist on Windows XP.
//Hopefully it doesn't do anything important.
int _except_handler4_common() {
	return 0;
}

static const BASS_PLUGININFO plugin_info = { BASS_VERSION, 0, NULL };

BOOL BASSDEF(DllMain)(HANDLE dll, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls((HMODULE)dll);
		if (HIWORD(BASS_GetVersion()) != BASSVERSION || !GetBassFunc()) {
			MessageBoxA(0, "Incorrect BASS.DLL version (" BASSVERSIONTEXT " is required)", "BASS", MB_ICONERROR | MB_OK);
			return FALSE;
		}
		crossfade_config_set(CF_MIXER, 0);
		crossfade_config_set(CF_MODE, CF_MANUAL);
		crossfade_config_set(CF_IN_PERIOD, 100);
		crossfade_config_set(CF_OUT_PERIOD, 100);
		crossfade_config_set(CF_IN_TYPE, CF_OUT_QUAD);
		crossfade_config_set(CF_OUT_TYPE, CF_OUT_QUAD);
		crossfade_config_set(CF_MIX, FALSE);
		crossfade_config_set(CF_BUFFER, FALSE);
		if (!crossfade_queue_create()) {
			return FALSE;
		}
		break;
	case DLL_PROCESS_DETACH:
		crossfade_queue_free();
		break;
	}
	return TRUE;
}

const VOID* BASSDEF(BASSplugin)(DWORD face) {
	switch (face) {
	case BASSPLUGIN_INFO:
		return (void*)&plugin_info;
	}
	return NULL;
}

BOOL BASSDEF(BASS_CROSSFADE_SetConfig)(const CF_ATTRIBUTE attrib, const DWORD value) {
	if (!crossfade_config_set(attrib, value)) {
		return FALSE;
	}
	crossfade_sync_refresh();
	return TRUE;
}

BOOL BASSDEF(BASS_CROSSFADE_GetConfig)(const CF_ATTRIBUTE attrib, DWORD* const value) {
	return crossfade_config_get(attrib, value);
}

HSTREAM BASSDEF(BASS_CROSSFADE_StreamCreate)(DWORD freq, DWORD chans, DWORD flags, void* user) {
	DWORD handle;
	DWORD mode;
	HSTREAM mixer = crossfade_mixer_create(freq, chans, flags, user);
	if (mixer) {
		if (crossfade_queue_peek(&handle)) {
			crossfade_queue_remove(handle);
			crossfade_config_get(CF_MODE, &mode);
			if (mode == CF_ALWAYS) {
				crossfade_mixer_add(handle, TRUE);
			}
			else {
				crossfade_mixer_add(handle, FALSE);
			}
		}
	}
	return mixer;
}

BOOL BASSDEF(BASS_CROSSFADE_StreamFadeIn)() {
	return crossfade_mixer_next(TRUE);
}

BOOL BASSDEF(BASS_CROSSFADE_StreamFadeOut)() {
	HSTREAM handle;
	if (!crossfade_mixer_playing()) {
		return FALSE;
	}
	if (!crossfade_mixer_peek(&handle)) {
		return FALSE;
	}
	if (!crossfade_mixer_remove(handle, TRUE)) {
		return FALSE;
	}
	return crossfade_queue_push(handle);
}

BOOL BASSDEF(BASS_CROSSFADE_StreamReset)(const DWORD flags) {
	HSTREAM handle;
	BOOL success = TRUE;
	BOOL fade_out;
	success &= crossfade_queue_clear();
	if (crossfade_mixer_playing()) {
		if (crossfade_mixer_peek(&handle)) {
			if (flags == CF_FADE_DEFAULT || flags == CF_FADE_OUT) {
				fade_out = TRUE;
			}
			else {
				fade_out = FALSE;
			}
			success &= crossfade_mixer_remove(handle, fade_out);
		}
	}
	return success;
}

DWORD* BASSDEF(BASS_CROSSFADE_GetChannels)(DWORD* const count) {
	DWORD position;
	DWORD mixer_count;
	DWORD queue_count;
	static HSTREAM handles[MAX_CHANNELS] = { 0 };
	HSTREAM* mixer_handles = crossfade_mixer_get_all(&mixer_count);
	HSTREAM* queue_handles = crossfade_queue_get_all(&queue_count);
	for (position = 0, *count = 0; position < mixer_count; position++, (*count)++) {
		handles[*count] = mixer_handles[position];
	}
	for (position = 0; position < queue_count; position++, (*count)++) {
		handles[*count] = queue_handles[position];
	}
	return handles;
}

BOOL BASSDEF(BASS_CROSSFADE_ChannelEnqueue)(const HSTREAM handle, const DWORD flags) {
	DWORD mode;
	BOOL success = FALSE;
	BOOL fade_in;
	if (!crossfade_mixer_playing()) {
		if (flags == CF_FADE_DEFAULT) {
			crossfade_config_get(CF_MODE, &mode);
			if (mode == CF_ALWAYS) {
				fade_in = TRUE;
			}
			else {
				fade_in = FALSE;
			}
		}
		else if (flags == CF_FADE_IN) {
			fade_in = TRUE;
		}
		else {
			fade_in = FALSE;
		}
		success = crossfade_mixer_add(handle, fade_in);
	}
	if (!success) {
		success = crossfade_queue_enqueue(handle);
	}
	return success;
}

BOOL BASSDEF(BASS_CROSSFADE_ChannelRemove)(const HSTREAM handle, const DWORD flags) {
	DWORD mix;
	BOOL success = TRUE;
	BOOL fade_out;
	crossfade_config_get(CF_MIX, &mix);
	//This probably isn't required.
	crossfade_sync_unregister(handle);
	if (flags == CF_FADE_DEFAULT || flags == CF_FADE_OUT) {
		fade_out = TRUE;
	}
	else {
		fade_out = FALSE;
	}
	if (mix) {
		crossfade_mixer_next(TRUE);
		success &= crossfade_mixer_remove(handle, fade_out) || crossfade_queue_remove(handle);
	}
	else {
		success &= crossfade_mixer_remove(handle, fade_out) || crossfade_queue_remove(handle);
		crossfade_mixer_next(TRUE);
	}
	return success;
}