#pragma once
#include <vector>
#include <Windows.h>
#include <WinUser.h>
#include "DesktopDefense.h"
#define CELL_ABOUTBROKEN 5
#define CELL_TOTALLYBROKEN 9 // 이 횟수 이상으로 맞으면 완전히 깨진다.

enum class CellState
{
	Clear, // 아무 것도 아니고 깨끗한 상태
	Registered, // 특정 광부에게 지정당한 상태 및 깨지고 있는 상태
	Broken, // 깨진 상태
};

/// <summary>
/// 맵 구조 중 한 칸을 표시하는 클래스입니다.
/// </summary>
class Cell
{
public:

	Cell();
	
	~Cell();

	CellState GetCellState();

	void SetCellState(CellState _state);

	int GetHittedCount();

	bool GetIsPaintedByAboutBroken();
	
	void SetIsPaintedByAboutBroken(bool _right);

	bool GetIsPaintedByBroken();

	void SetIsPaintedByBroken(bool _right);

	inline int GetIndexOfBrokenSprite() const { return m_IndexOfBrokenSprite; }

	inline void SetIndexOfBrokenSprite(int _index) { m_IndexOfBrokenSprite = _index; }

	void PlusHittedCount(); // 그 블럭을 치고 있는 Miner는 한 객체일 것임. 한 대 칠때마다 마이너에서 한 번 호출

	void CellTick(); // 나는 셀틱을 좋아하는 편이다.

private:

	// 현재 Cell이 어떤 상태인지 나타내는 멤버변수입니다. 
	CellState m_CellState;
	// Cell이 광부에게 몇 회 맞았는지 나타내는 멤버변수입니다. 이 변수와 m_CellState 변수로 매 Tick마다 분기합니다.
	int m_HittedCount;

	bool m_IsPaintedByAboutBroken; // 이 Cell이 깨진 후 마젠타로 색칠 되었나요 ..?

	bool m_IsPaintedByBroken;
	
	int m_IndexOfBrokenSprite; // AboutBroken 때 몇 번 인덱스의 스프라이트를 사용해 연출했는지 기억하고 Broken 상황일 때 똑같은 이미지를 뽑습니다.
};

/// <summary>
/// 게임이 진행되는 맵 전체를 총괄하는 클래스입니다.
/// </summary>
class Map
{
public:

	Map(HWND _hWnd, int _pixelX, int _pixelY); // 사용자 Windows의 핸들과 가로, 세로 픽셀 수를 받아 맵을 생성한다.

	~Map();

	void MapTick_Logic(); // 게임 오브젝트에 대한 모든 연산이 끝나고 마지막에 처리한다. (화면이 깨졌는지 안 깨졌는지 Check)

	void MapTick_Render(); // 위의 로직 업데이트가 끝나고 맵의 최종 상황을 화면 상에 출력합니다.

// private:

	Cell*** m_Map; // 그리드로 맵을 표현합시다.

	Engine::Sprite* m_BlueScreen; // 블루스크린 이미지

	Engine::Sprite* m_Desktop; // 캡쳐한 데스크탑 이미지

	// 깨지기 일보 직전 (3대 맞음), 완전히 깨짐 (5대 맞음) 이미지 Pair, Vector로 저장
	std::vector<pair<Engine::Sprite*, Engine::Sprite*>> m_BrokenSprites; // 마젠타 이미지들을 모아놓은 벡터. 랜덤으로 이미지를 여기서 뽑아서 m_Desktop에 붙인다. (TransparentBlt(RGB(0,255,0)))

	std::vector<Engine::Sprite*> m_RealBrokenAnimation;

	int m_BrokenCellCount;

	int m_MaxY;

	int m_MaxX;

	static const int m_PixelPerGrid = 40;
};

// GameWorld에 맵을 넣자 ..?
// 
// 윈도우 크리에이트 할 때 Map 생성하면서 블루스크린, 바탕화면, 깨진 이미지 전부 로드
// MapTick에서 화면 추우우우우우울려어어억