#include "UIManager.h"

#include <windows.h>
#include "Screen.h"
#include "Renderer.h"

extern GameObject* g_Player;

UIManager::UIManager(GameObject* _gameObject)
	:ComponentBase(_gameObject)
{
}

void UIManager::GameTimeTick()
{
	float _gameTime = Time::GetInstance()->GetGameTime();

	int _min = ((int)_gameTime) / 60;

	int _sec = ((int)_gameTime) % 60;

	string _minString = to_string(_min);

	string _secString = to_string(_sec);

	if (_minString.length() == 1)
		_minString = '0' + _minString;

	if (_secString.length() == 1)
		_secString = '0' + _secString;

	string _timeString = _minString + ':' + _secString;

	int _width = m_NumbersForTime[0]->GetBitmapInfo().bmWidth;

	int _startX = (GetSystemMetrics(SM_CXSCREEN) / 2) - 190;

	int _pixelY = GetSystemMetrics(SM_CYSCREEN);

	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
			Screen::GetInstance()->PrintSprite(m_NumbersForTime[10], Vector2D(_startX + i * (_width + 20), _pixelY - 120));
		else
			Screen::GetInstance()->PrintSprite(m_NumbersForTime[_timeString[i] - '0'], Vector2D(_startX + i * (_width + 20), _pixelY - 120));
	}
}

void UIManager::Tick()
{
	InventoryTick();
	ChangeWeaponCursor();
	MoveToMouseCursorTick();
	GameTimeTick();
}

// 0. 초기화
void UIManager::Initialize(Player* _Player)
{
	m_Player = _Player;
	// 선택한 무기에 따른 인벤토리 변화
	m_Select_WeaponSprite[0] = Sprite::LoadSprite(TEXT("Sprites/Select_Cusor.bmp"), Vector2D{ 0, 0 });
	m_Select_WeaponSprite[1] = Sprite::LoadSprite(TEXT("Sprites/Select_Hammer.bmp"), Vector2D{ 0, 0 });
	m_Select_WeaponSprite[2] = Sprite::LoadSprite(TEXT("Sprites/Select_HandGun.bmp"), Vector2D{ 0, 0 });
	m_Select_WeaponSprite[3] = Sprite::LoadSprite(TEXT("Sprites/Select_MiniGun.bmp"), Vector2D{ 0, 0 });
	m_Select_WeaponSprite[4] = Sprite::LoadSprite(TEXT("Sprites/Select_Laser.bmp"), Vector2D{ 0, 0 });

	int _pixelX = GetSystemMetrics(SM_CXSCREEN);
	int _pixelY = GetSystemMetrics(SM_CYSCREEN);

	// 인벤토리를 동적으로 사이즈 조절
	Vector2D _newScaler = Vector2D((double)_pixelX / (6 * m_Select_WeaponSprite[0]->GetBitmapInfo().bmWidth),
		(double)_pixelY / (16 * m_Select_WeaponSprite[0]->GetBitmapInfo().bmHeight));

	for (int i = 0; i < 5; i++)
		m_Select_WeaponSprite[i]->SetScaler(_newScaler);

	m_InventoryObject = new GameObject();
	m_InventoryPosition = { 0, 0 };
	m_InventoryObject->m_Position = m_InventoryPosition;
	m_InventoryObject->AddComponent<Renderer>();
	m_InventoryObject->GetComponent<Renderer>()->SetSprite(m_Select_WeaponSprite[m_Player->m_SelectWeapon]);

	m_WeaponCursorObject = new GameObject();
	m_WeaponCursorSprite[0] = Sprite::LoadSprite(TEXT("Sprites/Weapon_Cursor.bmp"), Vector2D{ .2, 1 });
	m_WeaponCursorSprite[1] = Sprite::LoadSprite(TEXT("Sprites/Weapon_Hammer.bmp"), Vector2D{ 0, 0 });
	m_WeaponCursorSprite[2] = Sprite::LoadSprite(TEXT("Sprites/Weapon_HandGun.bmp"), Vector2D{ 0, 1 });
	m_WeaponCursorSprite[3] = Sprite::LoadSprite(TEXT("Sprites/Weapon_Minigun.bmp"), Vector2D{ 0, 1 });
	m_WeaponCursorSprite[4] = Sprite::LoadSprite(TEXT("Sprites/Weapon_laser.bmp"), Vector2D{ 0, 1 });
	m_WeaponCursorObject->AddComponent<Renderer>();

	m_WeaponCursorAttackObject = new GameObject();
	m_WeaponCursorAttackSprite[0] = Sprite::LoadSprite(TEXT("Sprites/Weapon_Cursor_Attack.bmp"), Vector2D{ .2, 1 });
	m_WeaponCursorAttackSprite[1] = Sprite::LoadSprite(TEXT("Sprites/Weapon_Hammer_Attack.bmp"), Vector2D{ .5, .5 });
	m_WeaponCursorAttackSprite[2] = Sprite::LoadSprite(TEXT("Sprites/Weapon_HandGun_Attack.bmp"), Vector2D{ 0, 1 });
	m_WeaponCursorAttackSprite[3] = Sprite::LoadSprite(TEXT("Sprites/Weapon_Minigun_Attack.bmp"), Vector2D{ 0, 1 });
	m_WeaponCursorAttackSprite[4] = Sprite::LoadSprite(TEXT("Sprites/Weapon_laser_Attack.bmp"), Vector2D{ 0, 1 });
	m_WeaponCursorAttackObject->AddComponent<Renderer>();

 	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/0.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/1.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/2.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/3.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/4.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/5.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/6.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/7.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/8.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/9.bmp"), Vector2D(0.f, 0.f)));
	m_NumbersForTime.push_back(Sprite::LoadSprite(TEXT("Sprites/Number/10.bmp"), Vector2D(0.f, 0.f)));
}

void UIManager::InventoryTick()
{
	// 선택한 무기에 따라 인벤토리 UI를 갱신
	m_InventoryObject->GetComponent<Renderer>()->SetSprite(m_Select_WeaponSprite[m_Player->m_SelectWeapon]);
}

// WeaponTick -> UITick 해야한다.
void UIManager::ChangeWeaponCursor()
{
	// 선택한 무기에 따라 커서 스프라이트를 변경시킨다. (공격 상태면 공격 스프, 아니면 그냥 스프)
	bool _IsplayerAttackking = g_Player->GetComponent<Player>()->m_IsAttacking;

	if (!_IsplayerAttackking)
	{
		m_WeaponCursorAttackObject->GetComponent<Renderer>()->SetEnable(false);
		m_WeaponCursorObject->GetComponent<Renderer>()->SetEnable(true);
		m_WeaponCursorObject->GetComponent<Renderer>()->SetSprite(m_WeaponCursorSprite[m_Player->m_SelectWeapon]);
	}
	else // 공격 모션 중일 때는 이 스프라이트가 켜집니다.
	{
		m_WeaponCursorObject->GetComponent<Renderer>()->SetEnable(false);
		m_WeaponCursorAttackObject->GetComponent<Renderer>()->SetEnable(true);
		m_WeaponCursorAttackObject->GetComponent<Renderer>()->SetSprite(m_WeaponCursorAttackSprite[m_Player->m_SelectWeapon]);
	}
}

void UIManager::MoveToMouseCursorTick()
{
	// 현재 마우스 커서의 위치로 스프라이트를 이동시킨다.
	if (m_Player->m_SelectWeapon >= 2)
	{
		m_WeaponCursorObject->m_Position = { Screen::GetInstance()->ScreenToWorldPosition(Input::GetInstance()->GetMousePosition()) + Vector2D(30,-30) };
		m_WeaponCursorAttackObject->m_Position = { Screen::GetInstance()->ScreenToWorldPosition(Input::GetInstance()->GetMousePosition()) + Vector2D(30,-30) };
	}
	else
	{
		m_WeaponCursorObject->m_Position = { Screen::GetInstance()->ScreenToWorldPosition(Input::GetInstance()->GetMousePosition()) };
		m_WeaponCursorAttackObject->m_Position = { Screen::GetInstance()->ScreenToWorldPosition(Input::GetInstance()->GetMousePosition()) };
	}
}