#pragma once

#include "AnimationClip.h"
#include "ComponentBase.h"

namespace Engine
{
	class Animator : public ComponentBase
	{
	public:
		explicit Animator(GameObject* _gameObject);

		void Tick() override;

	private:
		// 현재 이 Animator Component에 등록된 애니메이션 클립입니다.
		AnimationClip* m_AnimationClip;

		// 등록된 애니메이션이 출력할 다음 스프라이트의 인덱스입니다. m_Animation의 size에 따른 범위 컨트롤이 필요합니다.
		int m_SpriteIndex = 0;

		// 가장 최근 스프라이트가 교체된 시간입니다. 이 시간과 각각의 프레임마다 현재 시간을 체크하고
		// m_TimeForSpriteChange와 비교 후 조건 만족 시 스프라이트를 넘기며 애니메이션을 재생합니다.
		float m_LastSpriteTime;

	public:
		// 이 컴포넌트 인스턴스를 가지고 있는 게임 오브젝트의 상태가 변경될 때 틀어야할 애니메이션 클립을 교체합니다.
		void SetAnimationClip(AnimationClip* _animationClip);
	};
}
