#pragma once

#include <map>
#include <Windows.h>
#include <vector>

#include "Vector2D.h"
#include <tchar.h>
#include <string>

namespace Engine
{

	class Sprite
	{
	public:
		// 생성자입니다.
		Sprite(HDC _hBitmapDC, HBITMAP _hBitmap, Vector2D _pivot = Vector2D::Center, Vector2D _scaler = Vector2D(1.f, 1.f));
		// 소멸자입니다.
		~Sprite();

	private:
		// 스프라이트를 가지고 있는 메모리 DC의 핸들입니다.
		HDC m_SpriteDC;
		// 스프라이트가 가지는 비트맵 이미지의 핸들입니다.
		HBITMAP m_HandleBitmap;
		// 스프라이트가 가지는 비트맵 이미지의 정보를 가지고 있는 오브젝트입니다.
		BITMAP m_BitmapInfo;

		// 스프라이트의 피봇입니다.
		Vector2D m_Pivot;

		/// <summary>
		/// 스프라이트를 표현할 때 어느 정도 크기로 바꾸어 출력할 것인지에 대한 비율입니다.
		///	이 값이 1이라면 원본 크기로, 0.5면 절반에 해당하는 크기로 출력합니다. 앞, 뒤 변수가 각각 X축, Y축에 대응합니다.
		/// </summary>
		Vector2D m_Scaler;

	public:
		inline HDC GetSpriteDC() const { return m_SpriteDC; }

		inline BITMAP GetBitmapInfo() const { return m_BitmapInfo; }

		inline Vector2D GetPivot() const { return m_Pivot; }

		inline void SetPivot(Vector2D _pivot) { this->m_Pivot = _pivot; }

		inline Vector2D GetScaler() const { return m_Scaler; }

		inline void SetScaler(Vector2D _value) { m_Scaler = _value; }

	private:
		

	public:
		static Sprite* LoadSprite(const TCHAR* _fileName, Vector2D _pivot = Vector2D::Center, Vector2D _scaler = Vector2D(1.f, 1.f));

		static Sprite* LoadSprite(HDC _hdc, const TCHAR* _fileName, Vector2D _pivot = Vector2D::Center, Vector2D _scaler = Vector2D(1.f, 1.f));
	};
}