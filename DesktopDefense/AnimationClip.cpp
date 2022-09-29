#include "AnimationClip.h"

using namespace Engine;

AnimationClip::AnimationClip(float _timeForSpriteChange, int _spriteCount, ...)
	: m_TimeForSpriteChange(_timeForSpriteChange)
{
	va_list _ap;
	va_start(_ap, _spriteCount);

	for (int i = 0; i < _spriteCount; i++)
	{
		Sprite* _sprite = va_arg(_ap, Sprite*);
		m_Sprites.emplace_back(_sprite);
	}

	va_end(_ap);
}
