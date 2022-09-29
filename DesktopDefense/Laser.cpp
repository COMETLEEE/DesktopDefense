#include "Laser.h"

#include "Audio.h"
#include "DesktopDefense.h"

extern Audio* _audio;

Laser::Laser(GameObject* _gameObject)
	: Weapon(_gameObject)
{
	this->m_Damage = LASER_DAMAGE;
	m_PenOption.PenStyle = EPenStyle::Solid, m_PenOption.PenColor = RGB(255, 0, 0), m_PenOption.PenWidth = 3;

	m_FireSound = AudioClip_LoadFromFile(_audio, L"Sounds/LayserSound.wav", false);
}

bool Laser::AttackTick()
{
	//this->m_GameObject->m_Position = Input::GetInstance()->GetMousePosition();

	auto _collider = this->m_GameObject->GetComponent<ColliderBase>();
	if (_collider == nullptr)
		return false;

	if (static_cast<int>(Input::GetInstance()->GetState(VK_LBUTTON)) & 0x01)
	{
		Screen::GetInstance()->PrintLineFromTo(m_StartPosition, m_EndPosition, m_PenOption);

		Audio_Play(_audio, m_FireSound, false);

		_collider->m_bEnable = true;
		return true;
	}
	else
	{
		_collider->m_bEnable = false;
		return false;
	}
}

void Laser::Attack(Miner* _miner)
{
	// 공격 쿨타임이 지나지 않았다면 공격을 실행할 수 없습니다.
	const float _gameTime = Time::GetInstance()->GetGameTime();
	if (_gameTime - m_LastAttackTime < m_TimePerAttack)
		return;

	m_LastAttackTime = _gameTime;

	_miner->Damage(m_Damage);
}
