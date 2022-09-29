#include "Animator.h"

#include "GameObject.h"
#include "Renderer.h"

using namespace Engine;

Animator::Animator(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
}

void Animator::Tick()
{
	ComponentBase::Tick();

	if (m_AnimationClip == nullptr)
		return;

	const float _gameTime = Time::GetInstance()->GetGameTime();

	const bool _bShouldFlip = _gameTime - m_LastSpriteTime >= m_AnimationClip->GetTimeForSpriteChange();
	if (_bShouldFlip == false)
		return;

	const auto& _sprites = m_AnimationClip->GetSprites();

	m_SpriteIndex = (m_SpriteIndex + 1) % _sprites.size();

	Renderer* _renderer = m_GameObject->GetComponent<Renderer>();
	if (_renderer != nullptr)
		_renderer->SetSprite(_sprites.at(m_SpriteIndex));

	m_LastSpriteTime = _gameTime;
}

void Animator::SetAnimationClip(AnimationClip* _animationClip)
{
	// 부모 게임 오브젝트의 상태에 맞는 애니메이션 클립을 매개변수로 받아 애니메이터에 등록된 클립을 변경합니다.
	m_AnimationClip = _animationClip;

	// 애니메이션의 시작점을 초기화합니다.
	m_SpriteIndex = 0;

	// 현재 시간으로 다시 애니메이션 시간을 초기화합니다.
	m_LastSpriteTime = Time::GetInstance()->GetGameTime();
}
