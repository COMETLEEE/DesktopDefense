#pragma once
#include "AudioClip.h"
#include "ComponentBase.h"
#include "DesktopDefense.h"
#include "GameObject.h"
#include "Miner.h"

using namespace Engine;

class Weapon : public ComponentBase
{
public:
	Weapon(GameObject* _gameObject);

	virtual bool AttackTick() = 0; // 순수 가상함수 (인터페이스만 제공)

	virtual void Attack(Miner* _miner) = 0;

	int m_Damage;

	float m_LastAttackTime; // 최근 데미지를 준 시간

	float m_TimePerAttack; // 공격 속도 (공격 1회당 몇 초의 시간이 걸리는지 컨트롤)

	AudioClip* m_FireSound;
};

// 요약 : 진짜 마우스 왼쪽 버튼으로 플레이어가 공격을 실행할 때 Weapon 별 Attack을 어떻게 수행할 지 함수를 만들면 됨
// (AttackTick()은 플레이어가 왼쪽 버튼을 누르고 있을 때 시행된다)

// 1. 윈도우 Mouse Cursor를 '크로스 헤어' 아이콘으로 바꾼다. (마우스 따라다니는 무기 스프라이트는 마우스 커서 위치 기준으로 알맞는 곳에 관리) ..?
// 2. 왼쪽 마우스 클릭이 될 때마다 (플레이어가 공격 동작을 수행할 때마다) 마우스 클릭된 포지션을 받아와 관리한다.
// 3. 무기 별로 그 위치에 공격 범위에 맞는 크기의 콜라이더, 이펙트 스프라이트와 렌더러를 가진 임시 게임 오브젝트를 생성 ..? => 다 서클 콜라이더 쓸 듯
// 4. 충돌검사 => 충돌 검사 후 위의 임시 생성된 콜라이더를 delete ..? 어렵다 .. 리플렉션 ..?
// 5. 충돌 true 시 => 광부로 타고 올라가서 hp 깎아버리고 광부에서는 hp <= 0 되면 광부 GameObject를 월드에서 삭제
// 6. 버튼 컴포넌트로 할까 ..

// m_Player에 .. 컴포넌트 다 박아
// Collider는 마우스 커서를 따라간다 (왼쪽 누르면 on, 왼쪽 안 누르면 off)
// 마우스 커서 좌표에 이펙트를 일으킨다.

// 왼쪽 누르면 AttackTick()을 수행 => 콜라이더 Tick에다가 콜백을 던져줌.
// 매 콜라이더 tick 끝날 때 m_bEnable을 꺼준다 ..?

// 충돌이 true이면 인자로 받은 콜백함수를 수행할텐데 ..
// 인자로 안 넘길꺼잖아 ..?
