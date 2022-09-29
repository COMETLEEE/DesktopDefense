#pragma once

#include <Windows.h>

#include "SingletonInstanceBase.h"

namespace Engine
{
	class GameInstanceBase : public SingletonInstanceBase<GameInstanceBase>
	{
	public:
		GameInstanceBase(HWND _hWnd);

		~GameInstanceBase() override;

		bool Tick();

		virtual bool OnWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	protected:
		virtual bool GameInstanceTick();

	protected:
		HWND m_hWnd;
		HDC m_hGeneralDC;

		class GameEngine* m_GameEngine;

		class GameWorld* m_GameWorld;

	public:
		HDC GetGeneralDC() const { return m_hGeneralDC; }

		GameEngine* GetGameEngine() const { return m_GameEngine; }

		GameWorld* GetGameWorld() const { return m_GameWorld; }
	};
}
