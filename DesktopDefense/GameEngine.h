#pragma once

#include <Windows.h>

namespace Engine
{
	class GameEngine
	{
	public:
		GameEngine(HWND _hWnd);

		~GameEngine();

		void Initialize();

		void PreTick();

		void PostTick();

	private:
		HWND m_hWnd;

		class Screen* m_Screen;

		class Input* m_Input;

		class Time* m_Time;

	public:
		inline Screen* GetScreen() const { return m_Screen; }

		inline Input* GetInput() const { return m_Input; }

		inline Time* GetTime() const { return m_Time; }
	};
}
