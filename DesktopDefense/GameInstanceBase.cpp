#include "GameInstanceBase.h"

#include "GameEngine.h"
#include "GameWorld.h"
#include "Screen.h"
#include "Time.h"

using namespace Engine;

GameInstanceBase::GameInstanceBase(HWND _hWnd)
	: SingletonInstanceBase<GameInstanceBase>(),
	  m_hWnd(_hWnd)
{
	m_hGeneralDC = GetDC(_hWnd);

	m_GameEngine = new GameEngine(_hWnd);
	m_GameEngine->Initialize();

	m_GameWorld = new GameWorld(this);
	m_GameWorld->Initialize();
}

GameInstanceBase::~GameInstanceBase()
{
	if (m_hGeneralDC != nullptr)
		ReleaseDC(m_hWnd, m_hGeneralDC);

	delete m_GameEngine;
	delete m_GameWorld;
}

bool GameInstanceBase::Tick()
{
	ReleaseDC(m_hWnd, m_hGeneralDC);
	m_hGeneralDC = GetDC(m_hWnd);

	m_GameEngine->PreTick();
	m_GameWorld->PreTick();

	bool _bEndGame = GameInstanceTick();
	m_GameWorld->Tick();

	m_GameEngine->PostTick();
	m_GameWorld->PostTick();

	return _bEndGame;
}

bool GameInstanceBase::OnWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_PAINT:
		m_GameEngine->GetScreen()->OnPaint();
		return true;
	}

	return false;
}

bool GameInstanceBase::GameInstanceTick()
{
	return false;
}
