#include "Hammer.h"

#include "Audio.h"

extern Audio* _audio;

Hammer::Hammer(GameObject* _gameObject)
	: Weapon(_gameObject)
{
	this->m_Damage = HAMMER_DAMAGE;

	m_FireSound = AudioClip_LoadFromFile(_audio, L"Sounds/HammerSound.wav", false);
}

bool Hammer::AttackTick()
{
	this->m_GameObject->m_Position = Input::GetInstance()->GetMousePosition();

	auto _collider = this->m_GameObject->GetComponent<ColliderBase>();
	if (_collider == nullptr)
		return false;

	if (Input::GetInstance()->GetState(VK_LBUTTON) == EKeyState::Down)
	{
		Audio_Play(_audio, m_FireSound, true);

		_collider->m_bEnable = true;
		return true;
	}
	else
	{
		_collider->m_bEnable = false;
		return false;
	}
}

void Hammer::Attack(Miner* _miner)
{
	// 공격 쿨타임이 지나지 않았다면 공격을 실행할 수 없습니다.
	const float _gameTime = Time::GetInstance()->GetGameTime();

	if (_gameTime - m_LastAttackTime < m_TimePerAttack)
		return;

	m_LastAttackTime = _gameTime;

	_miner->Damage(m_Damage);
}
