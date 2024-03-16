#ifndef BASSCROSSFADE_H
#define BASSCROSSFADE_H

#include "../bass/bass.h"
#include "../bass/bass_addon.h"

#define MAX_CHANNELS 10

typedef enum {
	CF_MIXER = 1,
	CF_MODE = 2,
	CF_IN_PERIOD = 3,
	CF_OUT_PERIOD = 4,
	CF_IN_TYPE = 5,
	CF_OUT_TYPE = 6,
	CF_MIX = 7,
	CF_BUFFER = 8
} CF_ATTRIBUTE;

typedef enum {
	CF_ALWAYS = 1,
	CF_MANUAL = 2
};

typedef enum {
	CF_LINEAR = 1,
	CF_IN_QUAD = 2,
	CF_OUT_QUAD = 3,
	CF_IN_EXPO = 4,
	CF_OUT_EXPO = 5
} CF_CURVE;

typedef enum {
	CF_FADE_DEFAULT = 0,
	CF_FADE_NONE = 1,
	CF_FADE_IN = 2,
	CF_FADE_OUT = 3
} CF_FADE;

BOOL BASSDEF(DllMain)(HANDLE dll, DWORD reason, LPVOID reserved);

const VOID* BASSDEF(BASSplugin)(DWORD face);

BOOL BASSDEF(BASS_CROSSFADE_SetConfig)(const CF_ATTRIBUTE attrib, const DWORD value);

BOOL BASSDEF(BASS_CROSSFADE_GetConfig)(const CF_ATTRIBUTE attrib, DWORD* const value);

HSTREAM BASSDEF(BASS_CROSSFADE_StreamCreate)(DWORD freq, DWORD chans, DWORD flags, void* user);

BOOL BASSDEF(BASS_CROSSFADE_StreamFadeIn)();

BOOL BASSDEF(BASS_CROSSFADE_StreamFadeOut)();

BOOL BASSDEF(BASS_CROSSFADE_StreamReset)(const DWORD flags);

DWORD* BASSDEF(BASS_CROSSFADE_GetChannels)(DWORD* const count);

BOOL BASSDEF(BASS_CROSSFADE_ChannelEnqueue)(const HSTREAM handle, const DWORD flags);

BOOL BASSDEF(BASS_CROSSFADE_ChannelRemove)(const HSTREAM handle, const DWORD flags);

#endif