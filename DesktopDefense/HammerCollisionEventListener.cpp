#include "HammerCollisionEventListener.h"

#include "Hammer.h"
#include "Time.h"
#include "Weapon.h"

using namespace Engine;

HammerCollisionEventListener::HammerCollisionEventListener(GameObject* _gameObject)
	: CollisionEventListenerBase(_gameObject)
{
}

void HammerCollisionEventListener::OnCollisionEnter(ColliderBase* _other)
{
	Hammer* _hammer = dynamic_cast<Hammer*>(m_GameObject->GetComponent<Weapon>());
	if (_hammer == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_hammer->Attack(_miner);
}

void HammerCollisionEventListener::OnCollisionStay(ColliderBase* _other)
{
}

void HammerCollisionEventListener::OnCollisionExit(ColliderBase* _other)
{
}