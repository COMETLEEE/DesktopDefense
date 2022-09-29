#include "Input.h"
#include "Player.h"
#include "Laser.h"


using namespace Engine;
extern GameObject* g_Player;
Player* g_PlayerComponent;
Laser* g_laserOfPlayer;

Input::Input(HWND _hWnd) :
	SingletonInstanceBase<Engine::Input>(),
	m_hWnd{ _hWnd },
	m_CurrentKeyStates{ 0 },
	m_PreviousKeyStates{ 0 },
	m_CurrentMousePosition{ Vector2D::Zero },
	m_PreviousMousePosition{ Vector2D::Zero },
	m_LastMouseDragStartPosition{ Vector2D::Zero },
	m_LastMouseDragEndPosition{ Vector2D::Zero },
	m_bMouseMove{ false }
{
}

void Input::Tick()
{
	BOOL _bSuccess;

	// 키보드 입력 정보를 갱신합니다.
	memcpy_s(m_PreviousKeyStates, VKEY_COUNT * sizeof(BYTE),
		m_CurrentKeyStates, VKEY_COUNT * sizeof(BYTE));

	_bSuccess = GetKeyboardState(m_CurrentKeyStates);
	
	Vector2D _prevMousePosition = m_PreviousMousePosition; 

	// 마우스 위치 정보를 갱신합니다.
	m_PreviousMousePosition = m_CurrentMousePosition;

	POINT _currentMousePoint;
	_bSuccess = GetCursorPos(&_currentMousePoint);
	_bSuccess = ScreenToClient(m_hWnd, &_currentMousePoint);
	m_CurrentMousePosition.x = _currentMousePoint.x;
	m_CurrentMousePosition.y = _currentMousePoint.y;



	// 마우스 드래그가 시작되고 끝나는 위치 정보를 갱신합니다.

	if (GetState(VK_LBUTTON) == EKeyState::Down)
	{
		m_LastMouseDragStartPosition = m_CurrentMousePosition;
		m_LastMouseDragEndPosition.x = m_LastMouseDragEndPosition.y = 0;
	}
	if ((GetState(VK_LBUTTON) == EKeyState::Hold) || (GetState(VK_LBUTTON) == EKeyState::Down))
	{
		if (g_Player->GetComponent<Player>()->m_SelectWeapon == 4)
		{
			g_PlayerComponent = g_Player->GetComponent<Player>();
			g_laserOfPlayer = g_PlayerComponent->m_WeaponObjectsSlot[g_PlayerComponent->m_SelectWeapon]->GetComponent<Laser>();
			g_laserOfPlayer->m_StartPosition = m_PreviousMousePosition;
			g_laserOfPlayer->m_EndPosition = m_CurrentMousePosition;
		}
	}
	else if (GetState(VK_LBUTTON) == EKeyState::Up)
	{
		m_LastMouseDragEndPosition = m_CurrentMousePosition;
		if (g_Player->GetComponent<Player>()->m_SelectWeapon == 4)
		{
			g_PlayerComponent = g_Player->GetComponent<Player>();
			g_laserOfPlayer = g_PlayerComponent->m_WeaponObjectsSlot[g_PlayerComponent->m_SelectWeapon]->GetComponent<Laser>();
			g_laserOfPlayer->m_EndPosition = m_LastMouseDragStartPosition;
		}
	}

	// 마우스가 움직였는지에 대한 여부를 갱신합니다.
	m_bMouseMove = m_CurrentMousePosition != m_PreviousMousePosition;
}

EKeyState Input::GetState(BYTE _vKey) const
{
	const bool _bPreviousPressed = m_PreviousKeyStates[_vKey] & 0b10000000;
	const bool _bCurrentPressed = m_CurrentKeyStates[_vKey] & 0b10000000;

	if (_bPreviousPressed == FALSE)
	{
		return _bCurrentPressed == FALSE ? EKeyState::None : EKeyState::Down;
	}
	else
	{
		return _bCurrentPressed == FALSE ? EKeyState::Up : EKeyState::Hold;
	}
}
