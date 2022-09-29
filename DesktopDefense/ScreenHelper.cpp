#include "ScreenHelper.h"

using namespace Engine;

bool ScreenHelper::CreateBitmap(HWND _hWnd, HBITMAP& _outHBitmap, HDC& _outHBitmapDC, RECT& _outBitmapRect)
{
	_outHBitmap = nullptr;
	_outHBitmapDC = nullptr;
	_outBitmapRect.top = 0;
	_outBitmapRect.bottom = 0;
	_outBitmapRect.left = 0;
	_outBitmapRect.right = 0;

	BOOL _result = FALSE;

	const HDC _hDC = GetDC(_hWnd);

	if (_hDC == nullptr)
		return false;

	RECT _clientRect;
	_result = GetClientRect(_hWnd, &_clientRect);
	if (_result == FALSE)
		return false;

	const HDC _newBitmapDC = CreateCompatibleDC(_hDC);
	const HBITMAP _newBitmap = CreateCompatibleBitmap(_hDC, _clientRect.right, _clientRect.bottom);
	SelectObject(_newBitmapDC, _newBitmap);

	FillRect(_newBitmapDC, &_clientRect, GetSysColorBrush(COLOR_WINDOW));

	ReleaseDC(_hWnd, _hDC);

	// 생성한 비트맵과 DC를 반환합니다.
	_outHBitmap = _newBitmap;
	_outHBitmapDC = _newBitmapDC;
	_outBitmapRect = _clientRect;

	return TRUE;
}
