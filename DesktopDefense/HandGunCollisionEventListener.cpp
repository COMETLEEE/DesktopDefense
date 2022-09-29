#include "HandGunCollisionEventListener.h"

#include "HandGun.h"
#include "Time.h"
#include "Weapon.h"

HandGunCollisionEventListener::HandGunCollisionEventListener(GameObject* _gameObject)
	: CollisionEventListenerBase(_gameObject)
{
}

void HandGunCollisionEventListener::OnCollisionEnter(ColliderBase* _other)
{
	HandGun* _handGun = dynamic_cast<HandGun*>(m_GameObject->GetComponent<Weapon>());
	if (_handGun == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_handGun->Attack(_miner);
}

void HandGunCollisionEventListener::OnCollisionStay(ColliderBase* _other)
{
	
}

void HandGunCollisionEventListener::OnCollisionExit(ColliderBase* _other)
{
}
