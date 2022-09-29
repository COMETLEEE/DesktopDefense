#pragma once
#include <fmod_common.h>
#include <stdbool.h>
#include <wchar.h>

struct AudioClip
{
	char* filePath;
	bool loop;

	FMOD_SOUND* sound;
	FMOD_CHANNEL* channel;
};

AudioClip* AudioClip_LoadFromFile(struct Audio* _audio, const wchar_t* _filePath, bool _loop);

void AudioClip_Release(AudioClip* _audioClip);
