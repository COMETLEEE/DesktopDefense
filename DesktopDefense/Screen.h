#pragma once
#include <Windows.h>

#include "Vector2D.h"
#include "Sprite.h"
#include "PaintShapeOption.h"
#include "SingletonInstanceBase.h"

namespace Engine
{
	class Screen : public SingletonInstanceBase<Screen>
	{
	public:
		Screen(HWND _hWnd, bool _bClearOnPreDraw);

		~Screen() override;

		void PreTick();

		void PostTick();

		void PreDraw();

		void PostDraw();

		void OnPaint();

		void CreateNewBitmap();

		void PrintSprite(Sprite* _sprite, Vector2D _worldPosition, COLORREF _eliminationColor = RGB(255, 0, 255));

		void PrintRectangle(Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option);

		void PrintRectangle(Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option);

		void PrintEllipse(Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option);

		void PrintEllipse(Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option);

		void PrintLineFromTo(Vector2D _from, Vector2D _to, const PaintShapeOption& _option);

		void PrintSpriteToScreenNow(Sprite* _sprite, Vector2D _worldPosition, COLORREF _eliminationColor = RGB(255, 0, 255));

		Vector2D WorldToScreenPosition(Vector2D _worldPosition) const;

		Vector2D ScreenToWorldPosition(Vector2D _screenPosition) const;

	private:
		const HWND m_hWnd;
		bool m_bClearOnPreDraw;

		bool m_bPainted;
		bool m_bDirty;
		HBITMAP m_hBitmap;
		HDC m_hBitmapDC;
		RECT m_ClientRect;

	public:
		inline HBITMAP GetScreenBitmap() const { return m_hBitmap; }

		inline HDC GetScreenBitmapDC() const { return m_hBitmapDC; }

		inline void SetDirty(bool _newDirty) { m_bDirty = _newDirty; }

	public:
		static bool PrintSpriteTo(HDC _hTargetDC, Sprite* _sprite, int _screenX, int _screenY, COLORREF _eliminationColor);

		static bool PrintRectangleTo(HDC _hTargetDC, Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option);

		static bool PrintRectangleTo(HDC _hTargetDC, Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option);

		static bool PrintEllipseTo(HDC _hTargetDC, Vector2D _minScreenPosition, Vector2D _maxScreenPosition, const PaintShapeOption& _option);

		static bool PrintEllipseTo(HDC _hTargetDC, Vector2D _centerPosition, int _width, int _height, const PaintShapeOption& _option);

		static bool PrintLineFromTo(HDC _hTargetDC, Vector2D _from, Vector2D _to, const PaintShapeOption& _option);

	private:
		static bool PrintSpriteToAlphaBlend(HDC _targetDC, Sprite* _sprite, int _screenX, int _screenY);

		static bool PrintSpriteToTransparentBlt(HDC _targetDC, Sprite* _sprite, int _screenX, int _screenY, COLORREF _eliminationColor);
	};
}
