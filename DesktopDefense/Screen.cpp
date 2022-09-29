#include "Screen.h"

#include <windowsx.h>
#include <wingdi.h>

#include "ScreenHelper.h"
#include "Map.h"

using namespace Engine;

extern Map* g_Map;

Screen::Screen(HWND _hWnd, bool _bClearOnPreDraw)
	: SingletonInstanceBase<Engine::Screen>(),
	  m_hWnd{_hWnd},
	  m_bClearOnPreDraw{_bClearOnPreDraw},
	  m_bPainted{true},
	  m_bDirty{false},
	  m_hBitmap{nullptr},
	  m_hBitmapDC{nullptr},
	  m_ClientRect{0, 0, 0, 0}
{
	CreateNewBitmap();
}

Screen::~Screen()
{
	if (m_hBitmapDC != nullptr)
		DeleteDC(m_hBitmapDC);

	if (m_hBitmap != nullptr)
		DeleteObject(m_hBitmap);
}

void Screen::PreTick()
{
	PreDraw();
}

void Screen::PostTick()
{
	if (m_bDirty == false)
		return;

	PostDraw();

	UpdateWindow(m_hWnd);
}

void Screen::PreDraw()
{
	// 이미 초기화된 경우 다시 실행하지 않습니다.
	if (m_bPainted == FALSE)
		return;

	m_bPainted = FALSE;


	// Client 영역 Rect를 얻습니다.
	RECT _clientRect;
	BOOL _success = GetClientRect(m_hWnd, &_clientRect);

	if (_success == FALSE)
		return;

	HDC _hDC = GetDC(m_hWnd);


	// 이전 프레임을 그릴 때 사용한 Bitmap과 BitmapDC, ClientRect를 얻습니다.
	// DC는 이번 프레임에 맞는 DC를 새로 생성합니다.
	const HDC _hBitmapLastFrameDC = m_hBitmapDC;
	const HBITMAP _hBitmapLastFrame = m_hBitmap;
	const RECT _clientRectLastFrame = m_ClientRect;


	/*
	 * Back Buffer로 사용할 Bitmap과 Bitmap에 대응하는 DC를 생성합니다.
	 * Back Buffer로 사용할 Bitmap은 그려지기 전 항상 배경색으로 채워져있어야 합니다.
	 */
	CreateNewBitmap();

	if (m_bClearOnPreDraw == FALSE && _hBitmapLastFrame != NULL)
	{
		// ClearOnPreDraw가 False이며, 이전에 그린 내용이 있을 때,
		// 이전의 화면 Bitmap 내용을 복사하여 다시 그립니다.
		_success = BitBlt(m_hBitmapDC, 0, 0, _clientRectLastFrame.right, _clientRectLastFrame.bottom,
		                  _hBitmapLastFrameDC, 0, 0, SRCCOPY);
	}
	else
	{
		// 그렇지 않은 경우, 전체를 배경색으로 칠합니다.
		FillRect(m_hBitmapDC, &_clientRect, GetSysColorBrush(COLOR_WINDOW));
	}

	// 이전 프레임에서 사용한 Bitmap과 DC를 해제합니다.
	if (_hBitmapLastFrame != nullptr)
		DeleteObject(_hBitmapLastFrame);

	if (_hBitmapLastFrameDC != nullptr)
		DeleteDC(_hBitmapLastFrameDC);

	// 이번 함수에서 잠시 사용되었던 DC를 해제합니다.
	ReleaseDC(m_hWnd, _hDC);
}

void Screen::PostDraw()
{
	InvalidateRect(m_hWnd, &m_ClientRect, FALSE);
}

void Screen::OnPaint()
{
	// ScreenBitmap에 변경사항이 있을 때에만 화면을 출력합니다.
	if (m_bDirty == FALSE)
		return;

	m_bDirty = FALSE;
	m_bPainted = TRUE;

	const RECT _clientRect = m_ClientRect;

	PAINTSTRUCT _ps;
	HDC _hDC = BeginPaint(m_hWnd, &_ps);

	BOOL _success = FALSE;

	// Bitmap의 내용을 DC에 그립니다.
	_success = BitBlt(_hDC, 0, 0, _clientRect.right, _clientRect.bottom, m_hBitmapDC, 0, 0, SRCCOPY);

	ValidateRect(m_hWnd, &_clientRect);

	EndPaint(m_hWnd, &_ps);
}

void Screen::CreateNewBitmap()
{
	HBITMAP _hBitmap;
	HDC _hBitmapDC;
	RECT _bitmapRect;
	BOOL _bResult = ScreenHelper::CreateBitmap(m_hWnd, _hBitmap, _hBitmapDC, _bitmapRect);

	if (_bResult == FALSE)
		return;

	m_hBitmap = _hBitmap;
	m_hBitmapDC = _hBitmapDC;
	m_ClientRect = _bitmapRect;
}

void Screen::PrintSprite(Sprite* _sprite, Vector2D _worldPosition, COLORREF _eliminationColor)
{
	if (_sprite == nullptr)
		return;

	const int _spriteWidth = _sprite->GetBitmapInfo().bmWidth;
	const int _spriteHeight = _sprite->GetBitmapInfo().bmHeight;
	// 여기가 수정되었습니다.
	const Vector2D _pivotAppliedWorldPosition{Vector2D::GetPivotAppliedPosition(_worldPosition, _sprite->GetPivot(), _spriteWidth * _sprite->GetScaler().x, _spriteHeight * _sprite->GetScaler().y)};
	const Vector2D _spriteWorldPosition{_pivotAppliedWorldPosition.x, _pivotAppliedWorldPosition.y + _spriteHeight * _sprite->GetScaler().y};
	const Vector2D _spriteScreenPosition{WorldToScreenPosition(_spriteWorldPosition)};
	// 여기가 수정되었습니다.
	Screen::PrintSpriteTo(m_hBitmapDC, _sprite,
	                      static_cast<int>(_spriteScreenPosition.x), static_cast<int>(_spriteScreenPosition.y), _eliminationColor);
	SetDirty(true);
}

void Screen::PrintRectangle(Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option)
{
	PrintRectangleTo(m_hBitmapDC, _minScreenPosition, _maxScreenPosition, _option);
	SetDirty(true);
}

void Screen::PrintRectangle(Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option)
{
	PrintRectangleTo(m_hBitmapDC, _centerPosition, _width, _height, _option);
	SetDirty(true);
}

void Screen::PrintEllipse(Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option)
{
	PrintEllipseTo(m_hBitmapDC, _minScreenPosition, _maxScreenPosition, _option);
	SetDirty(true);
}

void Screen::PrintEllipse(Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option)
{
	PrintEllipseTo(m_hBitmapDC, _centerPosition, _width, _height, _option);
	SetDirty(true);
}

void Screen::PrintLineFromTo(Vector2D _from, Vector2D _to, const PaintShapeOption& _option)
{
	PrintLineFromTo(m_hBitmapDC, _from, _to, _option);
	SetDirty(true);
}

void Screen::PrintSpriteToScreenNow(Sprite* _sprite, Vector2D _worldPosition, COLORREF _eliminationColor)
{
	if (_sprite == nullptr)
		return;

	const RECT _clientRect = m_ClientRect;

	HDC _hDC = GetDC(m_hWnd);

	BOOL _success = FALSE;

	Vector2D _scaler = _sprite->GetScaler();

	// Bitmap의 내용을 DC에 그립니다.

	// _success = BitBlt(_hDC, 0, 0, _sprite->GetBitmapInfo().bmWidth * _scaler.x, _sprite->GetBitmapInfo().bmHeight*_scaler.y, _sprite->GetSpriteDC(), 0, 0, SRCCOPY);

	TransparentBlt(_hDC, 0, 0, _sprite->GetBitmapInfo().bmWidth * _scaler.x, _sprite->GetBitmapInfo().bmHeight * _scaler.y, _sprite->GetSpriteDC(), 0, 0, _sprite->GetBitmapInfo().bmWidth, _sprite->GetBitmapInfo().bmHeight, _eliminationColor);

	ValidateRect(m_hWnd, &_clientRect);

	ReleaseDC(m_hWnd, _hDC);
}

Vector2D Screen::WorldToScreenPosition(Vector2D _worldPosition) const
{
	const double _screenPositionX = _worldPosition.x;
	const double _screenPositionY = m_ClientRect.bottom - _worldPosition.y;
	const Vector2D _screenPosition{_screenPositionX, _screenPositionY};

	return _screenPosition;
}

Vector2D Screen::ScreenToWorldPosition(Vector2D _screenPosition) const
{
	const double _worldPositionX = _screenPosition.x;
	const double _worldPositionY = m_ClientRect.bottom - _screenPosition.y;
	const Vector2D _worldPosition{_worldPositionX, _worldPositionY};

	return _worldPosition;
}

bool Screen::PrintSpriteTo(HDC _hTargetDC, Sprite* _sprite, int _screenX, int _screenY, COLORREF _eliminationColor)
{
	return PrintSpriteToTransparentBlt(_hTargetDC, _sprite, _screenX, _screenY, _eliminationColor);
}

bool Screen::PrintRectangleTo(HDC _hTargetDC, Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option)
{
	HPEN _hNewPen = CreatePen(static_cast<int>(_option.PenStyle), _option.PenWidth, _option.PenColor);
	HPEN _hOldPen = SelectPen(_hTargetDC, _hNewPen);

	HBRUSH _hNewBrush = nullptr;
	switch (_option.BrushFillType)
	{
	case EBrushFillType::None:
		_hNewBrush = nullptr;
		break;

	case EBrushFillType::Solid:
		_hNewBrush = CreateSolidBrush(_option.BrushColor);
		break;

	case EBrushFillType::HatchPattern:
		_hNewBrush = CreateHatchBrush(0, _option.BrushColor);
		break;
	}

	HBRUSH _hOldBrush = SelectBrush(_hTargetDC, _hNewBrush);

	const bool _success = Rectangle(_hTargetDC,
	                                static_cast<int>(_minScreenPosition.x), static_cast<int>(_minScreenPosition.y),
	                                static_cast<int>(_maxScreenPosition.x), static_cast<int>(_maxScreenPosition.y));

	SelectPen(_hTargetDC, _hOldPen);
	DeletePen(_hNewPen);

	SelectBrush(_hTargetDC, _hOldBrush);
	DeleteBrush(_hNewBrush);

	return _success;
}

bool Screen::PrintRectangleTo(HDC _hTargetDC, Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option)
{
	const int _halfWidth = _width / 2;
	const int _halfHeight = _height / 2;
	const Vector2D _minScreenPosition{_centerPosition.x - _halfWidth, _centerPosition.y - _halfHeight};
	const Vector2D _maxScreenPosition{_centerPosition.x + _halfWidth, _centerPosition.y + _halfHeight};

	return PrintRectangleTo(_hTargetDC, _minScreenPosition, _maxScreenPosition, _option);
}

bool Screen::PrintEllipseTo(HDC _hTargetDC, Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option)
{
	HPEN _hNewPen = CreatePen(static_cast<int>(_option.PenStyle), _option.PenWidth, _option.PenColor);
	HPEN _hOldPen = SelectPen(_hTargetDC, _hNewPen);

	HBRUSH _hNewBrush = nullptr;
	switch (_option.BrushFillType)
	{
	case EBrushFillType::None:
		_hNewBrush = nullptr;
		break;

	case EBrushFillType::Solid:
		_hNewBrush = CreateSolidBrush(_option.BrushColor);
		break;

	case EBrushFillType::HatchPattern:
		_hNewBrush = CreateHatchBrush(0, _option.BrushColor);
		break;
	}

	HBRUSH _hOldBrush = SelectBrush(_hTargetDC, _hNewBrush);

	const bool _success = Ellipse(_hTargetDC,
	                              static_cast<int>(_minScreenPosition.x), static_cast<int>(_minScreenPosition.y),
	                              static_cast<int>(_maxScreenPosition.x), static_cast<int>(_maxScreenPosition.y));

	SelectPen(_hTargetDC, _hOldPen);
	DeletePen(_hNewPen);

	SelectBrush(_hTargetDC, _hOldBrush);
	DeleteBrush(_hNewBrush);

	return _success;
}

bool Screen::PrintEllipseTo(HDC _hTargetDC, Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option)
{
	const int _halfWidth = _width / 2;
	const int _halfHeight = _height / 2;
	const Vector2D _minScreenPosition{_centerPosition.x - _halfWidth, _centerPosition.y - _halfHeight};
	const Vector2D _maxScreenPosition{_centerPosition.x + _halfWidth, _centerPosition.y + _halfHeight};

	return PrintRectangleTo(_hTargetDC, _minScreenPosition, _maxScreenPosition, _option);
}

bool Screen::PrintLineFromTo(HDC _hTargetDC, Vector2D _from, Vector2D _to, const PaintShapeOption& _option)
{
	HPEN _hNewPen = CreatePen(static_cast<int>(_option.PenStyle), _option.PenWidth, _option.PenColor);
	HPEN _hOldPen = SelectPen(_hTargetDC, _hNewPen);

	// 선을 긋는 작업이기 때문에 브러쉬는 고려하지 않는다.

	bool _success = MoveToEx(_hTargetDC,
	                         static_cast<int>(_from.x), static_cast<int>(_from.y), nullptr);

	if (_success == false)
		return false;

	_success = LineTo(_hTargetDC,
	                  static_cast<int>(_to.x), static_cast<int>(_to.y));

	SelectPen(_hTargetDC, _hOldPen);
	DeletePen(_hNewPen);

	return _success;
}


bool Screen::PrintSpriteToAlphaBlend(HDC _targetDC, Sprite* _sprite, int _screenX, int _screenY)
{
	const int _spriteWidth = _sprite->GetBitmapInfo().bmWidth;
	const int _spriteHeight = _sprite->GetBitmapInfo().bmHeight;
	const Vector2D _spriteScaler = _sprite->GetScaler();

	constexpr BLENDFUNCTION _bf{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

	return AlphaBlend(_targetDC, _screenX, _screenY, _spriteWidth * _spriteScaler.x, _spriteHeight * _spriteScaler.y,
	                  _sprite->GetSpriteDC(), 0, 0, _spriteWidth, _spriteHeight, _bf);
}

bool Screen::PrintSpriteToTransparentBlt(HDC _targetDC, Sprite* _sprite, int _screenX, int _screenY, COLORREF _eliminationColor)
{
	const int _spriteWidth = _sprite->GetBitmapInfo().bmWidth;
	const int _spriteHeight = _sprite->GetBitmapInfo().bmHeight;
	const Vector2D _spriteScaler = _sprite->GetScaler();

	return TransparentBlt(_targetDC, _screenX, _screenY, _spriteWidth * _spriteScaler.x, _spriteHeight * _spriteScaler.y,
	                      _sprite->GetSpriteDC(), 0, 0, _spriteWidth, _spriteHeight, _eliminationColor);
}
