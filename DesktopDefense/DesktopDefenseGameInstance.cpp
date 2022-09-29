#include "DesktopDefenseGameInstance.h"
#include <Time.h>
#include <random>
#include "Animator.h"
#include "Renderer.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "Button.h"
#include "Input.h"
#include "Miner.h"
#include "UIManager.h"
#include "Player.h"
#include "Map.h"
#include "Laser.h"
#include "Audio.h"
#include "AudioClip.h"
#include <algorithm>

extern Map* g_Map;
extern GameObject* g_Player;
extern bool g_IsClicked; // Test Code
extern bool g_IsDoubleClicked; // Test Code
extern GameObject* g_Player;
extern Audio* _audio;
AudioClip* _IngameBGM;

extern int g_k;
extern int g_y;
int g_Mineridx;

DesktopDefenseGameInstance::DesktopDefenseGameInstance(HWND _hWnd)
	: GameInstanceBase(_hWnd)
{
	srand(time(NULL));
	HDC _hDC = GetDC(_hWnd);
	m_TestSprite = Sprite::LoadSprite(TEXT("Sprites/test-thumb-32bit.bmp"), Vector2D::Center);

	// 테스트용 게임 오브젝트를 생성하고, Renderer와 Animator 컴포넌트를 추가합니다.
	m_TestGameObject = new GameObject();
	m_TestGameObject->m_Position = Vector2D{300, 300};

	auto _renderer = m_TestGameObject->AddComponent<Engine::Renderer>(); //Renderer가 있어야 Animator가 돌아감.

	ReleaseDC(_hWnd, _hDC);

	// 맵은 이 전에 WM_CREATE 메시지에서 생성합니다.
	// 유일한 플레이어 생성
	g_Player = new GameObject();
	g_Player->AddComponent<Player>();
	g_Player->GetComponent<Player>()->Initialize();
	g_Player->AddComponent<CircleCollider>(); // 이 콜라이더가 마우스 포지션으로 계속 업데이트됨.
	g_Player->AddComponent<Renderer>();

	m_UIManager = new GameObject();
	m_UIManager->AddComponent<UIManager>();
	m_UIManager->GetComponent<UIManager>()->Initialize(g_Player->GetComponent<Player>());
	CreateMiner();

	Player* _playerComponent = g_Player->GetComponent<Player>();
	Laser* _laserOfPlayer = _playerComponent->m_WeaponObjectsSlot[_playerComponent->m_SelectWeapon]->GetComponent<Laser>();

	_IngameBGM = AudioClip_LoadFromFile(_audio, L"Sounds/Bgm/ingame_bgm.wav", false);
	Audio_Play(_audio, _IngameBGM, false);
}

DesktopDefenseGameInstance::~DesktopDefenseGameInstance()
{
	delete m_TestSprite;
}

//마이너를 그냥 만들기만 했다.
void DesktopDefenseGameInstance::CreateMiner()
{
	g_Mineridx = 0;
	for (int i = 0; i < 200; i++)
	{
		GameObject* _newGameObject = new GameObject();
		_newGameObject->AddComponent<Miner>();
		_newGameObject->AddComponent<Renderer>();
		_newGameObject->AddComponent<Animator>();
		/*_newGameObject->AddComponent<CircleCollider>();
		_newGameObject->GetComponent<CircleCollider>()->m_Radius = 40;*/

		m_VectorMiner.push_back(_newGameObject);
		m_DisabledMinerSet.insert(i);
		_newGameObject->GetComponent<Miner>()->SetMinerIdx(g_Mineridx);
		g_Mineridx++;
	}
}

void DesktopDefenseGameInstance::OnMiner()
{
	if (m_DisabledMinerSet.size() == 0)
		return;

	auto _beginIter = m_DisabledMinerSet.begin();

	int _index = *_beginIter;
	m_DisabledMinerSet.erase(_beginIter);

	auto _miner = m_VectorMiner[_index];

	_miner->GetComponent<Miner>()->Initialize(RandomPosition(), 70.f, 10.f, 5);
	_miner->SetEnable(true);
}

void DesktopDefenseGameInstance::OffMiner(int mineridx)
{
	m_DisabledMinerSet.insert(mineridx);
}

Vector2D DesktopDefenseGameInstance::RandomPosition()
{
	m_Quadrant = rand() % 4 + 1;
	switch (m_Quadrant)
	{
	case FIRST_QUADRANT:
		{
			m_RespawnX = rand() % (GetSystemMetrics(SM_CXSCREEN) + 1);
			m_RespawnY = - 100;
			return Vector2D(m_RespawnX, m_RespawnY);
		}
		break;
	case SECOND_QUADRANT:
		{
			m_RespawnX = - 100;
			m_RespawnY = rand() % (GetSystemMetrics(SM_CYSCREEN) + 1);
			return Vector2D(m_RespawnX, m_RespawnY);
		}
		break;
	case THIRD_QUADRANT:
		{
			m_RespawnX = rand() % (GetSystemMetrics(SM_CXSCREEN) + 1);
			m_RespawnY = GetSystemMetrics(SM_CYSCREEN) + 100;
			return Vector2D(m_RespawnX, m_RespawnY);
		}
		break;
	case FOURTH_QUADRANT:
		{
			m_RespawnX = GetSystemMetrics(SM_CXSCREEN) + 100;
			m_RespawnY = rand() % (GetSystemMetrics(SM_CYSCREEN) + 1);
			return Vector2D(m_RespawnX, m_RespawnY);
		}
		break;
	default:
		return Vector2D::Zero;
		break;
	}

	/*m_RespawnX = rand() % (GetSystemMetrics(SM_CXSCREEN) + 1);
	m_RespawnY = rand() % (GetSystemMetrics(SM_CYSCREEN) + 1);*/

	return Vector2D(m_RespawnX, m_RespawnY);
}

bool DesktopDefenseGameInstance::GameInstanceTick()
{
	Screen::GetInstance()->PrintSprite(m_TestSprite, Vector2D{100, 100});

	// 맵의 렌더링을 담당하는 함수입니다 (BlueScreen -> (로직 처리 후 마젠타 덧칠) -> Desktop)
	g_Map->MapTick_Logic();
	g_Map->MapTick_Render();

	OnMiner();
	// 오른쪽으로 초당 20만큼 이동합니다.
	// 지금 당장은 거리 1당 화면 1픽셀만큼이라고 생각하면 됩니다.
	// (나중에 줌인/아웃 또는 카메라 오브젝트가 생겨나면서 달라질 수 있습니다. 구현을 어디까지 하게될지는 미정입니다.)
	Vector2D _direction = Vector2D{1, 1}.Normalize();
	float _moveSpeed = 20.f;

	//m_TestGameObject->m_Position = m_TestGameObject->m_Position + _direction * _moveSpeed * GetGameEngine()->GetTime()->GetDeltaTime();

	// 
	//m_ButtonOne = new GameObject();
	//Button* _button = m_ButtonOne->AddComponent<Button>(); //Button* 반환
	//_button->m_StartPosition = Vector2D{0, 0};
	//_button->m_EndPosition = Vector2D{50, 50};

	////GetGameEngine()->GetInput()->GetMousePosition();
	//if (GetGameEngine()->GetInput()->GetState(VK_LBUTTON) == EKeyState::Down)
	//{
	//	if (_button->OnClick(GetGameEngine()->GetInput()->GetMousePosition()))
	//		GetGameEngine()->GetScreen()->PrintLineFromTo(Vector2D{50, 50}, Vector2D{150, 150},
	//		                                              PaintShapeOption{EPenStyle::Solid, 5, RGB(255, 0, 0), EBrushFillType::Solid, RGB(100, 100, 100)});
	//}

	return false;
}
