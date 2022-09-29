#include "Sprite.h"

#include "GameInstanceBase.h"

using namespace Engine;

// 핸들 디시
extern HDC g_hdc;

Sprite::Sprite(HDC _hBitmapDC, HBITMAP _hBitmap, Vector2D _pivot, Vector2D _scaler)
	: m_SpriteDC(_hBitmapDC),
	m_HandleBitmap(_hBitmap),
	m_Pivot(_pivot),
	m_Scaler(_scaler)
{
	// 해당하는 스프라이트가 가지는 비트맵 이미지의 정보를 가져옵니다.
	BITMAP _bitmapInfo;
	GetObject(_hBitmap, sizeof(BITMAP), &_bitmapInfo);

	m_BitmapInfo = _bitmapInfo;
}

Sprite::~Sprite()
{
	// 해당 스프라이트를 가지고 있는 메모리 DC를 해제합니다.
	DeleteDC(m_SpriteDC);
	// 해당 스프라이트가 가지고 있는 비트맵 이미지를 해제합니다.
	DeleteObject(m_HandleBitmap);
}

Sprite* Sprite::LoadSprite(const TCHAR* _fileName, Vector2D _pivot, Vector2D _scaler)
{
	return LoadSprite(GameInstanceBase::GetInstance()->GetGeneralDC(), _fileName, _pivot, _scaler);
}

std::map<std::wstring, HBITMAP> LoadedBitmaps;
std::map<std::wstring, HDC> LoadedBitmapDCs;
Sprite* Sprite::LoadSprite(HDC _hDC, const TCHAR* _fileName, Vector2D _pivot, Vector2D _scaler)
{
	// 만약, 지금 매개로 들어온 파일 명과 일치하는 파일이 한 번이라도 불러온 적 있다면,
	// 실제로 불러오는 코드는 무시하고, 이미 불러온걸 재사용하자 이거지.

	HDC _hBitmapDC;
	// Bitmap 이미지를 출력할 DC 공간 준비 (메모리 공간)

	HBITMAP _hBitmap;

	std::wstring _fileNameString = _fileName;

	auto _loadedBitmapIter = LoadedBitmaps.find(_fileNameString);
	if (_loadedBitmapIter == LoadedBitmaps.end())
	{
		_hBitmapDC = CreateCompatibleDC(_hDC);
		_hBitmap = (HBITMAP)LoadImage(nullptr, _fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(_hBitmapDC, _hBitmap); // m_TextureDC에 m_BitmapInfo 끼워넣기

		LoadedBitmaps.insert({ _fileNameString, _hBitmap });		
		LoadedBitmapDCs.insert({ _fileNameString, _hBitmapDC });
	}
	else
	{
		_hBitmapDC = LoadedBitmapDCs.at(_fileNameString);
		_hBitmap = LoadedBitmaps.at(_fileName);
	}

	if (_hBitmap == nullptr)
	{
		// 이미지 로드 실패
		// 로드 실패에 따른 예외처리를 실시합니다.
		return nullptr;
	}

	Sprite* _newSprite = new Sprite(_hBitmapDC, _hBitmap, _pivot, _scaler);

	return _newSprite;
}
