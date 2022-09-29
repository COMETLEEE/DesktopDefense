#include "AudioClip.h"

#include <stdlib.h>
#include <fmod.h>

#include "Audio.h"
#include "Helper.h"
#include <tchar.h>

char* WcharStringToCharString(const wchar_t* _source)
{
	const int _sourceLength = wcslen(_source);
	const int _outStringByteSize = sizeof(char) * _sourceLength + 1;

	size_t _numOfCharConverted;

	char* _outString = (char*)malloc(_outStringByteSize);
	wcstombs_s(&_numOfCharConverted, _outString, _outStringByteSize, _source, _outStringByteSize - 1);

	return _outString;
}

AudioClip* AudioClip_LoadFromFile(Audio* _audio, const wchar_t* _filePath, bool _loop)
{
	if (_audio == NULL || _audio->system == NULL)
		return NULL;

	if (_filePath == NULL)
		return NULL;

	FMOD_SOUND* _sound = NULL;

	char* _filePathMbs = WcharStringToCharString(_filePath);
	FMOD_MODE _mode = FMOD_2D | (_loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	FMOD_RESULT _result = FMOD_System_CreateSound(_audio->system, _filePathMbs, FMOD_DEFAULT, NULL, &_sound);

	if (_result != FMOD_OK)
		return NULL;

	AudioClip* _outClip = (AudioClip*)malloc(sizeof(AudioClip));
	_outClip->filePath = _filePathMbs;
	_outClip->loop = _loop;
	_outClip->sound = _sound;
	_outClip->channel = NULL;

	return _outClip;
}

void AudioClip_Release(AudioClip* _audioClip)
{
	if (_audioClip == NULL)
		return;

	if (_audioClip->sound != NULL)
		FMOD_Sound_Release(_audioClip->sound);

	if (_audioClip->filePath != NULL)
		free(_audioClip->filePath);

	free(_audioClip);
}

bool AudioClip_IsPlaying(AudioClip* _audioClip)
{
	if (_audioClip == NULL || _audioClip->channel == NULL)
		return false;

	FMOD_BOOL _isPlaying;
	const FMOD_RESULT _result = FMOD_Channel_IsPlaying(_audioClip->channel, &_isPlaying);

	if (_result != FMOD_OK)
		return false;

	return _isPlaying == true;
}
