#include "HandGun.h"

#include "Audio.h"

extern Audio* _audio;

HandGun::HandGun(GameObject* _gameObject)
	: Weapon(_gameObject)
{
	this->m_Damage = HANDGUN_DAMAGE;

	m_FireSound = AudioClip_LoadFromFile(_audio, L"Sounds/HandGunSound.wav", false);
}

bool HandGun::AttackTick()
{
	auto _collider = this->m_GameObject->GetComponent<ColliderBase>();
	if (_collider == nullptr)
		return false;

	if (Input::GetInstance()->GetState(VK_LBUTTON) == EKeyState::Down)
	{
		Audio_Play(_audio, m_FireSound, true);

		_collider->m_bEnable = true;
		return true;
	}
	else
	{
		_collider->m_bEnable = false;
		return false;
	}
}

void HandGun::Attack(Miner* _miner)
{
	// 공격 쿨타임이 지나지 않았다면 공격을 실행할 수 없습니다.
	const float _gameTime = Time::GetInstance()->GetGameTime();
	if (_gameTime - m_LastAttackTime < m_TimePerAttack)
		return;

	m_LastAttackTime = _gameTime;

	_miner->Damage(m_Damage);
}

// 요약 : 진짜 마우스 왼쪽 버튼으로 플레이어가 공격을 실행할 때 Weapon 별 Attack을 어떻게 수행할 지 함수를 만들면 됨
// (AttackTick()은 플레이어가 왼쪽 버튼을 누르고 있을 때 시행된다)

// 0. ChangeWeapon에서 커서를 해당 무기에 맞게 바꾼다.
// 1. 윈도우 Mouse Cursor를 '크로스 헤어' 아이콘으로 바꾼다. (마우스 따라다니는 무기 스프라이트는 마우스 커서 위치 기준으로 알맞는 곳에 관리) ..?
// 2. 왼쪽 마우스 클릭이 될 때마다 (플레이어가 공격 동작을 수행할 때마다) 마우스 클릭된 포지션을 받아와 관리한다.
// 3. 무기 별로 그 위치에 공격 범위에 맞는 크기의 콜라이더, 이펙트 스프라이트와 렌더러를 가진 임시 게임 오브젝트를 생성 ..? => 다 서클 콜라이더 쓸 듯
// 4. 충돌검사 => 충돌 검사 후 위의 임시 생성된 콜라이더를 delete ..? 어렵다 .. 리플렉션 ..?
// 5. 충돌 true 시 => 광부로 타고 올라가서 hp 깎아버리고 광부에서는 hp <= 0 되면 광부 GameObject를 월드에서 삭제
// 6. 버튼 컴포넌트로 할까 ..
