#pragma once
#include <fmod_common.h>
#include <stdbool.h>

#include "AudioClip.h"

struct Audio
{
	FMOD_SYSTEM* system;
};

Audio* Audio_Create(int _maxChannelCount);

void Audio_Release(Audio* _audio);

bool Audio_IsPlaying(AudioClip* _audioClip);

bool Audio_Play(Audio* _audio, AudioClip* _audioClip, bool _forcePlay);

bool Audio_Stop(AudioClip* _audioClip);