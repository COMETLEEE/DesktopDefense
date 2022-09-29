#include "Miner.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "DesktopDefense.h"
#include "DesktopDefenseGameInstance.h"
#include "Player.h"
#include <random>
#include <time.h>

extern Map* g_Map;
extern GameObject* g_Player;

Miner::Miner(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
	//m_MinerCount++;
}
Miner::~Miner()
{
	//m_MinerCount--;
	delete m_MoveAnimationClip;
	delete m_DigAnimationClip;
	delete m_DieAnimationClip;
}

void Miner::Tick()
{
	MinerState();
}

void Miner::Initialize(Vector2D _initialPosition, float _moveSpeed, float _digtime, int _hp)
{
	m_GameObject->m_Position = _initialPosition;
	m_CurDigX = rand() % g_Map->m_MaxX;
	m_CurDigY = rand() % g_Map->m_MaxY;
	m_DigPosition = Screen::GetInstance()->ScreenToWorldPosition(Vector2D((double)m_CurDigX * g_Map->m_PixelPerGrid - MINER_DEVIATION, (double)m_CurDigY * g_Map->m_PixelPerGrid + MINER_DEVIATION));
	m_MoveSpeed = _moveSpeed;
	m_Hp = _hp;
	m_DigTime = _digtime;
	m_State = EMinerState::Move;
	m_PreviousState = EMinerState::None;
	m_PreviusDigTime = 0.f;
	//Gameobject를 비활성화하면 component가 Tick이 안돈다.(비활성화 해주는거랑 다름)
	this->m_GameObject->SetEnable(false);


	m_HitTime = 0.f;
	g_Map->m_Map[m_CurDigY][m_CurDigX]->SetCellState(CellState::Registered);
	m_Zero = Sprite::LoadSprite(TEXT("Sprites/Miner_0.bmp"), Vector2D::Zero);
	m_Scaler = Vector2D((double)MINER_SCALE / (double)m_Zero->GetBitmapInfo().bmWidth, (double)MINER_SCALE / (double)m_Zero->GetBitmapInfo().bmHeight);

	//Vector2D _zeroScaler = Vector2D::One;

	m_MoveAnimationClip = new AnimationClip(.3, 3, Sprite::LoadSprite(TEXT("Sprites/Miner_0.bmp"), Vector2D::Zero, m_Scaler),
		Sprite::LoadSprite(TEXT("Sprites/Miner_1.bmp"), Vector2D::Zero, m_Scaler), Sprite::LoadSprite(TEXT("Sprites/Miner_2.bmp"), Vector2D::Zero, m_Scaler));
	m_DigAnimationClip = new AnimationClip(.3, 3, Sprite::LoadSprite(TEXT("Sprites/1.bmp"), Vector2D::Zero, m_Scaler),
		Sprite::LoadSprite(TEXT("Sprites/2.bmp"), Vector2D::Zero, m_Scaler), Sprite::LoadSprite(TEXT("Sprites/3.bmp"), Vector2D::Zero, m_Scaler));
	m_DieAnimationClip = new AnimationClip(.3, 4, Sprite::LoadSprite(TEXT("Sprites/test1.bmp"), Vector2D::Zero),
		Sprite::LoadSprite(TEXT("Sprites/test2.bmp"), Vector2D::Zero), Sprite::LoadSprite(TEXT("Sprites/test3.bmp"), Vector2D::Zero), Sprite::LoadSprite(TEXT("Sprites/test4.bmp"), Vector2D::Zero));
}

void Miner::MinerState()
{
	SetRectanglePos();

	if (m_PreviousState != m_State)
	{
		switch (m_State)
		{
		case EMinerState::None:
			break;
		case EMinerState::Move:
		{
			m_GameObject->GetComponent<Animator>()->SetAnimationClip(m_MoveAnimationClip);
			m_PreviousState = m_State;
		}
		break;
		case EMinerState::Dig:
		{
			m_PreviusDigTime =
				Time::GetInstance()->GetGameTime();
			m_GameObject->GetComponent<Animator>()->SetAnimationClip(m_DigAnimationClip);
			m_PreviousState = m_State;
			m_HitTime = Time::GetInstance()->GetGameTime();
		}
		break;
		case EMinerState::Die:
		{
			m_GameObject->GetComponent<Animator>()->SetAnimationClip(m_DieAnimationClip);
			//m_GameObject->GetComponent<Engine::Renderer>()->SetSprite(Sprite::LoadSprite(TEXT("Sprites/test1.bmp"), Vector2D::Zero));

			m_PreviusDigTime =
				Time::GetInstance()->GetGameTime();
			m_PreviousState = m_State;
		}
		case EMinerState::Create:
		{
			dynamic_cast<DesktopDefenseGameInstance*>(DesktopDefenseGameInstance::GetInstance())->OnMiner();
			m_PreviousState = m_State;
		}
		break;
		}
	}

	switch (m_State)
	{
	case EMinerState::None:
		break;
	case EMinerState::Move:
	{
		Move();
	}
	break;
	case EMinerState::Dig:
	{
		Dig(Time::GetInstance()->GetGameTime() - m_PreviusDigTime);
	}
	break;
	case EMinerState::Die:
	{
		OnDie(Time::GetInstance()->GetGameTime() - m_PreviusDigTime);
	}
	break;
	}

	Vector2D _mousePos = Input::GetInstance()->GetMousePosition();

	Vector2D _mouseReal = Screen::GetInstance()->ScreenToWorldPosition(_mousePos);

	if ((g_Player->GetComponent<Player>()->m_SelectWeapon == 0) || (g_Player->GetComponent<Player>()->m_SelectWeapon == 1) || (g_Player->GetComponent<Player>()->m_SelectWeapon == 2))
	{
		if ((Input::GetInstance()->GetState(VK_LBUTTON) == EKeyState::Down) && (CheckMiner(_mouseReal)))
			m_Hp -= g_Player->GetComponent<Player>()->GetEquippedWeapon()->m_Damage;
	}
	else
	{
		if (CheckMiner(_mouseReal) && ((Input::GetInstance()->GetState(VK_LBUTTON) == EKeyState::Down) || (Input::GetInstance()->GetState(VK_LBUTTON) == EKeyState::Hold)))
			m_Hp -= g_Player->GetComponent<Player>()->GetEquippedWeapon()->m_Damage;
	}
}

//FSM
//광부의 움직임을 담당함
void Miner::Move()
{
	if (this->m_Hp <= 0)
		m_State = EMinerState::Die;
	Vector2D _direction = (m_DigPosition - m_GameObject->m_Position).Normalize();

	const float _distanceToDestination = m_GameObject->m_Position.DistanceTo(m_DigPosition);
	if (_distanceToDestination > 0.5f)
	{
		const float _deltaDistance = m_MoveSpeed * Time::GetInstance()->GetDeltaTime();

		// 목적지까지 남은 거리가 짧은 경우,
		// 목적지를 넘어가지 않도록 합니다.
		const float _desiredDeltaDistance = _distanceToDestination < _deltaDistance ? _distanceToDestination : _deltaDistance;

		m_GameObject->m_Position = m_GameObject->m_Position + _direction * _desiredDeltaDistance;
	}
	else
		m_State = EMinerState::Dig;
}

//광부가 죽었을때를 담당함
void Miner::OnDie(float _curtime)
{
	if (_curtime >= 1.0f)
	{
		this->m_GameObject->SetEnable(false);
		dynamic_cast<DesktopDefenseGameInstance*>(DesktopDefenseGameInstance::GetInstance())->OffMiner(m_Mineridx);

		//GameWorld::GetInstance()->Destroy(this->m_GameObject);
		//m_State = EMinerState::Create;
	}
}

//광부가 캘때 행동을 담당함
void Miner::Dig(float _curtime)
{
	if (this->m_Hp <= 0)
		m_State = EMinerState::Die;
	bool CellCheck = true;

	int x;
	int y;

	if (_curtime >= m_DigTime)
	{
		while (CellCheck)
		{
			x = rand() % g_Map->m_MaxX;
			y = rand() % g_Map->m_MaxY;
			if (g_Map->m_Map[y][x]->GetCellState() == CellState::Clear)
			{
				m_State = EMinerState::Move;
				CellCheck = !CellCheck;
				m_CurDigX = x;
				m_CurDigY = y;
				g_Map->m_Map[y][x]->SetCellState(CellState::Registered);
				m_DigPosition = Screen::GetInstance()->ScreenToWorldPosition(Vector2D((double)m_CurDigX * g_Map->m_PixelPerGrid - 35, (double)m_CurDigY * g_Map->m_PixelPerGrid + 35));;
			}
			// 레지스터로 바꿔준당
		}
	}

	if (Time::GetInstance()->GetGameTime() - this->m_HitTime >= 0.9f)
	{
		g_Map->m_Map[m_CurDigY][m_CurDigX]->PlusHittedCount();
		this->m_HitTime = Time::GetInstance()->GetGameTime();
		/*m_Hp -= 1;*/
	}
}

void Miner::Damage(int damage)
{
	this->m_Hp -= damage;

	if (this->m_Hp <= 0)
		m_State = EMinerState::Die;
}

void Miner::SetRectanglePos()
{
	m_Zero = Sprite::LoadSprite(TEXT("Sprites/Miner_0.bmp"), Vector2D::Zero);
	m_LeftBottomPos = this->m_GameObject->m_Position;
	m_RightTopPos = Vector2D(m_LeftBottomPos.x + m_Zero->GetBitmapInfo().bmWidth * m_Scaler.x, m_LeftBottomPos.y +  m_Zero->GetBitmapInfo().bmHeight * m_Scaler.y);
}

bool Miner::CheckMiner(Vector2D curpos)
{
	if ((m_LeftBottomPos.x <= curpos.x) && (m_LeftBottomPos.y <= curpos.y) && (curpos.x <= m_RightTopPos.x) && (curpos.y <= m_RightTopPos.y))
		return true;
	else
		return false;
}

//파괴되는 Window의 좌표를 리턴함
Vector2D DestoryWindow()
{
	return Vector2D::Zero;
}
