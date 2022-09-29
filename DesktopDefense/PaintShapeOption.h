#pragma once

#include <Windows.h>

enum class EPenStyle : UINT8
{
	None = PS_NULL,
	Solid = PS_SOLID,
	Dash = PS_DASH,
	Dot = PS_DOT,
	DashDot = PS_DASHDOT,
	DashDotDot = PS_DASHDOTDOT,
	InsiderFrame = PS_INSIDEFRAME,
};

enum class EBrushFillType : UINT8
{
	None = 0,
	Solid = 1,
	HatchPattern = 2,
	Pattern = 3,
};

struct PaintShapeOption
{
public:
	EPenStyle PenStyle;
	int PenWidth;
	COLORREF PenColor;
	EBrushFillType BrushFillType;
	COLORREF BrushColor;
};
