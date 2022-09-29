#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "Sprite.h"

using namespace std;
using namespace Engine;

namespace Engine
{
	class AnimationClip
	{
	public:
		AnimationClip() = default;

		AnimationClip(float _timeForSpriteChange, int _spriteCount, ...);

	private:
		// 여러 장의 스프라이트가 일렬로 배치되어 만들어진 애니메이션입니다.
		vector<Sprite*> m_Sprites;

		// 이 애니메이션 클립의 고유한 특성입니다. 이 애니메이션에 있는 스프라이트가 어떤 시간 간격마다 
		// 교체되어야 하는지에 대한 정보입니다.
		float m_TimeForSpriteChange;
	public:
		inline vector<Sprite*> GetSprites() const { return m_Sprites; }

		inline float GetTimeForSpriteChange() const { return m_TimeForSpriteChange; }

		inline void SetTimeForSpriteChange(float _value) { m_TimeForSpriteChange = _value; }
	};
}
