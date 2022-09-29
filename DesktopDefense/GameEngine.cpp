#include "GameEngine.h"

#include "DesktopDefense.h"

using namespace Engine;

GameEngine::GameEngine(HWND _hWnd)
	: m_hWnd(_hWnd)
{
	Initialize();
}

GameEngine::~GameEngine()
{
	delete m_Input;
	delete m_Screen;
	delete m_Time;
}

void GameEngine::Initialize()
{
	m_Input = new Input(m_hWnd);
	m_Screen = new Screen(m_hWnd, true);
	m_Time = new Time();
}

void GameEngine::PreTick()
{
	m_Screen->PreTick();

	m_Input->Tick();

	m_Time->PreTick();
}

void GameEngine::PostTick()
{
	m_Time->PostTick();

	m_Screen->PostTick();
}
