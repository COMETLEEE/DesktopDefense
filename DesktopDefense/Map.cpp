#include "Map.h"
#include "CircleCollider.h"
#include "Audio.h"

extern Audio* _audio;
extern AudioClip*_IngameBGM;

Cell::Cell()
{
	m_CellState = CellState::Clear;

	m_HittedCount = 0;
	
	m_IsPaintedByAboutBroken = false;
	
	m_IsPaintedByBroken = false;

	m_IndexOfBrokenSprite = 0;
}

Cell::~Cell()
{

}

CellState Cell::GetCellState()
{
	return this->m_CellState;
}

void Cell::SetCellState(CellState _state)
{
	m_CellState = _state;

	return;
}

int Cell::GetHittedCount()
{
	return this->m_HittedCount;
}

bool Cell::GetIsPaintedByAboutBroken()
{
	return this->m_IsPaintedByAboutBroken;
}

void Cell::SetIsPaintedByAboutBroken(bool _right)
{
	this->m_IsPaintedByAboutBroken = _right;
}

bool Cell::GetIsPaintedByBroken()
{
	return this->m_IsPaintedByBroken;
}

void Cell::SetIsPaintedByBroken(bool _right)
{
	this->m_IsPaintedByBroken = _right;
}

void Cell::PlusHittedCount()
{
	m_HittedCount++;

	return;
}

void Cell::CellTick()
{
	// 상태를 체크한다.
	// 체크된 상태에 따라서 Screen에 해당하는 이미지의 렌더링을 요청한다.
	// MapTick()에서 실행됩니다.

	CellState _nowCellState = m_CellState;

	switch (_nowCellState)
	{
		case CellState::Clear:
		{
			// 뭐 없을까 ..?
			break;
		}

		case CellState::Registered:
		{
			if (this->m_HittedCount >= CELL_TOTALLYBROKEN)
			{
				this->m_CellState = CellState::Broken;
			}

			break;
		}

		case CellState::Broken:
		{
			// 뭐 없을까 ..?
			break;
		}

		default:
			break;
	}
}

// 맵을 생성합니다.
// Map(GetSystemMetrics(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); 로 호출
Map::Map(HWND _hWnd, int _pixelX, int _pixelY)
{
	// 맵 구조 생성 (10픽셀마다 1그리드)
	m_MaxY = _pixelY / Map::m_PixelPerGrid;

	m_MaxX = _pixelX / Map::m_PixelPerGrid;

	m_Map = new Cell** [m_MaxY];

	for (int i = 0; i < m_MaxY; i++)
		m_Map[i] = new Cell* [m_MaxX];

	for (int y = 0; y < m_MaxY; y++)
	{
		for (int x = 0; x < m_MaxX; x++)
		{
			m_Map[y][x] = new Cell();
		}
	}

	m_BrokenCellCount = 0;
	
	// 맵에서 관리하는 스프라이트를 로드한다. (데스크탑, 블루스크린, 깨짐 연출용 마젠타)
	HDC _hScrDC, _hMemDC;
	HBITMAP _hBitmap;

	_hScrDC = CreateDCA("DISPLAY", NULL, NULL, NULL);
	_hMemDC = CreateCompatibleDC(_hScrDC);
	_hBitmap = CreateCompatibleBitmap(_hScrDC, _pixelX, _pixelY);
	SelectObject(_hMemDC, _hBitmap);
	BitBlt(_hMemDC, 0, 0, _pixelX, _pixelY, _hScrDC, 0, 0, SRCCOPY);

	// 데스크탑 이미지 로드
	m_Desktop = new Engine::Sprite(_hMemDC, _hBitmap, Engine::Vector2D(0.f, 0.f));

	DeleteDC(_hScrDC);

	HDC _hDC = GetDC(_hWnd);

	// 블루스크린과 깨지는 연출용 이미지 로드

	m_BlueScreen = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/BLUESCREEN16by9.bmp"), Engine::Vector2D(0.f, 0.f));

	Engine::Vector2D _newScaler = Engine::Vector2D((double)_pixelX / (double)m_BlueScreen->GetBitmapInfo().bmWidth, (double)_pixelY / (double)m_BlueScreen->GetBitmapInfo().bmHeight);

	m_BlueScreen->SetScaler(_newScaler);

	// 깨지는 연출용 이미지 로드 (for문으로 해도 되지만 그냥 무지성으로 하고 싶어서 함) => 나중에 경로 바꿀텐데 눈물날듯
	Sprite* _zeroOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/0-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _zeroTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/0-2.bmp"), Engine::Vector2D(0.f, 0.f));

	Engine::Vector2D _scalerForBrokenSprite = Engine::Vector2D((double)m_PixelPerGrid / (double)_zeroOne->GetBitmapInfo().bmWidth, (double)m_PixelPerGrid / (double)_zeroOne->GetBitmapInfo().bmHeight);

	_zeroOne->SetScaler(_scalerForBrokenSprite);

	_zeroTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _zeroOne, _zeroTwo });

	Sprite* _oneOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/1-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _oneTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/1-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_oneOne->SetScaler(_scalerForBrokenSprite);

	_oneTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _oneOne, _oneTwo });

	Sprite* _TwoOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/2-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _TwoTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/2-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_TwoOne->SetScaler(_scalerForBrokenSprite);

	_TwoTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _TwoOne, _TwoTwo });

	Sprite* _ThreeOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/3-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _ThreeTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/3-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_ThreeOne->SetScaler(_scalerForBrokenSprite);

	_ThreeTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _ThreeOne, _ThreeTwo });

	Sprite* _FourOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/4-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _FourTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/4-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_FourOne->SetScaler(_scalerForBrokenSprite);

	_FourTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _FourOne, _FourTwo });

	Sprite* _FiveOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/5-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _FiveTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/5-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_FiveOne->SetScaler(_scalerForBrokenSprite);

	_FiveTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _FiveOne, _FiveTwo });

	Sprite* _SixOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/6-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _SixTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/6-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_SixOne->SetScaler(_scalerForBrokenSprite);

	_SixTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _SixOne, _SixTwo });

	Sprite* _SevenOne = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/7-1.bmp"), Engine::Vector2D(0.f, 0.f));

	Sprite* _SevenTwo = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/7-2.bmp"), Engine::Vector2D(0.f, 0.f));

	_SevenOne->SetScaler(_scalerForBrokenSprite);

	_SevenTwo->SetScaler(_scalerForBrokenSprite);

	m_BrokenSprites.push_back({ _SevenOne, _SevenTwo });

	Sprite* _RealBroken = Engine::Sprite::LoadSprite(_hDC, TEXT("Sprites/Broken/REALBROKEN.bmp"), Engine::Vector2D(0.f, 0.f));
	
	Vector2D _overScaler = Vector2D((double)_pixelX / _RealBroken->GetBitmapInfo().bmWidth, (double)_pixelY / _RealBroken->GetBitmapInfo().bmHeight);

	_RealBroken->SetScaler(_overScaler);

	m_RealBrokenAnimation.push_back(_RealBroken);
}

// 맵을 제거합니다. 동적으로 할당되어 있던 모든 자료들을 해제합니다.
Map::~Map()
{
	for (int i = 0; i < m_MaxY; i++)
		delete [] m_Map[i];

	delete[] m_Map;

	for (int i = 0; i < m_BrokenSprites.size(); i++)
	{
		delete m_BrokenSprites[i].first;
		delete m_BrokenSprites[i].second;
	}

	delete m_BlueScreen;
	delete m_Desktop;
}

void Map::MapTick_Logic()
{	
	for (int y = 0; y < m_MaxY; y++)
	{
		for (int x = 0; x < m_MaxX; x++)
		{
			if (m_Map[y][x]->GetCellState() != CellState::Broken) // 이미 Broken된 Cell이라면 더 이상 업데이트할 상황이 없다.
			{
				m_Map[y][x]->CellTick(); // 깨진 부위 업데이트 = 데스크탑 화면 업데이트
			}
		}
	}

	if (m_BrokenCellCount >= (int)m_MaxX * m_MaxY * 0.3f) // 일정 비율의 Cell이 깨지면
	{
		// 게임 종료시키자 .. 

		// 오디오 스탑

		Audio_Stop(_IngameBGM);

		Screen::GetInstance()->PrintSpriteToScreenNow(m_RealBrokenAnimation[0], Vector2D{ 0, 0 }, RGB(255, 0, 255));

		Sleep(1000);
		
		Screen::GetInstance()->PrintSpriteToScreenNow(m_BlueScreen, Vector2D{ 0, 0 }); // 블루스크린 화면에 냅다 꽃기

		Screen::GetInstance()->PrintSpriteToScreenNow(m_RealBrokenAnimation[0], Vector2D{ 0, 0 }, RGB(255, 0, 255));

		while (true)
		{
			Sleep(100000);
		}

		//Screen::GetInstance()->PrintSpriteToScreenNow(m_RealBrokenAnimation[0], Vector2D{ 0, 0 });
		
		// 렌더에서 요청해서 금 가게하고 와장창, 블루스크린 -> 끝
	}
}

void Map::MapTick_Render()
{
	Screen::GetInstance()->PrintSprite(this->m_BlueScreen, Vector2D{0, 0});
	
	for (int y = 0; y < m_MaxY; y++)
	{
		for (int x = 0; x < m_MaxX; x++)
		{
			if (!m_Map[y][x]->GetIsPaintedByAboutBroken() && m_Map[y][x]->GetCellState() == CellState::Registered && m_Map[y][x]->GetHittedCount() >= CELL_ABOUTBROKEN)
			{
				m_Map[y][x]->SetIsPaintedByAboutBroken(true); // AboutBroken 이미지 출력하였다는 Flag On
				// 금간 이미지를 랜덤으로 뽑아서 DesktopDC에 그린다. (PrintSpriteTo 함수는 screen 좌표에 쏩니다)
				m_Map[y][x]->SetIndexOfBrokenSprite(rand() % m_BrokenSprites.size()); // 준비된 파괴 이미지 중 랜덤으로 하나를 뽑는다.
				Screen::GetInstance()->PrintSpriteTo(m_Desktop->GetSpriteDC(), m_BrokenSprites[m_Map[y][x]->GetIndexOfBrokenSprite()].first, m_PixelPerGrid * x, m_PixelPerGrid * y, RGB(0, 255, 0));
			}
			else if (!m_Map[y][x]->GetIsPaintedByBroken() && m_Map[y][x]->GetCellState() == CellState::Broken && m_Map[y][x]->GetHittedCount()>= CELL_TOTALLYBROKEN)
			{
				m_Map[y][x]->SetIsPaintedByBroken(true); // TotallyBroken 이미지를 출력하였다는 Flag On
;				Screen::GetInstance()->PrintSpriteTo(m_Desktop->GetSpriteDC(), m_BrokenSprites[m_Map[y][x]->GetIndexOfBrokenSprite()].second, m_PixelPerGrid * x, m_PixelPerGrid * y, RGB(0, 255, 0));
				m_BrokenCellCount++;
			}
		}
	}

	Screen::GetInstance()->PrintSprite(this->m_Desktop, Vector2D{0, 0});
}

// 일반 (여기서 Map에 대한 로직을 마지막에 처리한다)
// 물리 
// 렌더링 

// 광부 별로 다음 타겟을 정하고 깨는 대충 알고리즘
// CellState::Clear인 Cell이 나올 때가지 m_Map에서 rand(m_MaxX -1, m_MaxY -1) ..
// 찾으면 m_Map[y][x]->SetCellState(CellState::Registered)로 변경해주고 그 곳으로 이동
// 그 좌표 (픽셀 좌표)은 대충 x * 10 + 5 , y * 10 + 5임 (좌상단 0,0 / 우하단 m_Map->SM_CXSCREEN, m_Map->SM_CYSCREEN)
// 찾아서 공격 모드 들어가면 '곡괭이로 땅을 찍는 Sprite'가 나올 때 g_Map->m_Map[y][x] (공격하기로 지정한 Cell)->PlusHittedCount() 한 번 호출
// 다음 MinerTick에서 지금 지정한 Cell이 Breaked로 바뀌었다면 다음 셀로 가면 댐