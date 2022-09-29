#include "Player.h"

#include <Windows.h>

#include "CircleCollider.h"
#include "Input.h"

#include "Cursor.h"
#include "CursorCollisionEventListener.h"
#include "Hammer.h"
#include "HammerCollisionEventListener.h"
#include "HandGun.h"
#include "HandGunCollisionEventListener.h"
#include "MiniGun.h"
#include "MiniGunCollisionEventListener.h"
#include "Laser.h"
#include "LaserCollisionEventListener.h"
#include "Audio.h"


using namespace Engine;

extern Audio* _audio;

Player::Player(GameObject* _gameobject)
	: ComponentBase(_gameobject)
{
}

void Player::Initialize()
{
	GameObject* _cursor = new GameObject();
	_cursor->AddComponent<Cursor>();
	_cursor->AddComponent<CursorCollisionEventListener>();

	GameObject* _hammer = new GameObject();
	_hammer->AddComponent<Hammer>();
	_hammer->AddComponent<HammerCollisionEventListener>();

	GameObject* _handGun = new GameObject();
	_handGun->AddComponent<HandGun>();
	_handGun->AddComponent<HandGunCollisionEventListener>();

	GameObject* _miniGun = new GameObject();
	_miniGun->AddComponent<MiniGun>();
	_miniGun->AddComponent<MiniGunCollisionEventListener>();

	GameObject* _laser = new GameObject();
	_laser->AddComponent<Laser>();
	_laser->AddComponent<LaserCollisionEventListener>();
	_laser->AddComponent<Renderer>();

	m_WeaponObjectsSlot[0] = _cursor;
	m_WeaponObjectsSlot[1] = _hammer;
	m_WeaponObjectsSlot[2] = _handGun;
	m_WeaponObjectsSlot[3] = _miniGun;
	m_WeaponObjectsSlot[4] = _laser;
	m_IsAttacking = false;
	// 최초 시작 시 무기 설정
	m_SelectWeapon = 0;
}

void Player::Tick()
{
	this->m_IsAttacking = AttackTick();

	ChangeWeaponTick();
}

/// <summary>
/// 공격 명령을 현재 선택된 weapon에게 내리는 함수
/// </summary>
bool Player::AttackTick()
{
	Weapon* _equippedWeapon = GetEquippedWeapon();
	if (_equippedWeapon == nullptr)
		return false;

	_equippedWeapon->m_GameObject->m_Position = Screen::GetInstance()->ScreenToWorldPosition(Input::GetInstance()->GetMousePosition());

	return _equippedWeapon->AttackTick();
}

/// <summary>
/// 무기를 변경하는 함수
/// - 입력값에 따라 이에 해당하는 무기로 변경
/// </summary>
void Player::ChangeWeaponTick()
{
	for (size_t i = 1; i <= SLOT_COUNT; i++)
	{
		if (Input::GetInstance()->GetState(0x30 + i) == EKeyState::Down)
		{
			// GetEquippedWeapon()->m_bEnable = false;
			m_SelectWeapon = i - 1;
			// GetEquippedWeapon()->m_bEnable = true;
			break;
		}
	}
}
