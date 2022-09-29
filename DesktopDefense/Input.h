#pragma once

#include "DesktopDefense.h"
#include "SingletonInstanceBase.h"

namespace Engine
{
	constexpr int VKEY_COUNT = 256;

	/// <summary>
	/// 키의 상태를 나타내는 열거형입니다.
	///	가장 하위 비트는 현재 프레임에 키가 눌려있었던 상태인지를 나타내고,
	///	밑에서 2번째 비트는 이전 프레임에 키가 눌려있었던 상태인지를 나타냅니다.
	/// </summary>
	enum class EKeyState
	{
		// 00
		None = 0x00,
		// 16
		Up = 0x10,
		// 01
		Down = 0x01,
		// 17
		Hold = 0x11
	};

	class Input : public SingletonInstanceBase<Input>
	{
	public:
		Input(HWND _hWnd);

		void Tick();

		EKeyState GetState(BYTE _vKey) const;

	private:
		const HWND m_hWnd;

		BYTE m_CurrentKeyStates[VKEY_COUNT];
		BYTE m_PreviousKeyStates[VKEY_COUNT];
		Vector2D m_CurrentMousePosition;
		Vector2D m_PreviousMousePosition;
		Vector2D m_LastMouseDragStartPosition;
		Vector2D m_LastMouseDragEndPosition;
		bool m_bMouseMove;

	public:
		inline Vector2D GetMousePosition() const { return m_CurrentMousePosition; }

		inline Vector2D GetLastMouseDragStartPosition() const { return m_LastMouseDragStartPosition; }

		inline Vector2D GetLastMouseDragEndPosition() const { return m_LastMouseDragEndPosition; }
	};
}