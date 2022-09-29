#pragma once

#include <Windows.h>

namespace Engine
{
	class ScreenHelper
	{
	public:
		static bool CreateBitmap(HWND _hWnd, HBITMAP& _outHBitmap, HDC& _outHBitmapDC, RECT& _outBitmapRect);
	};
}
