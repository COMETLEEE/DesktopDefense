#include "LaserCollisionEventListener.h"

#include "Laser.h"
#include "Time.h"
#include "Weapon.h"

LaserCollisionEventListener::LaserCollisionEventListener(GameObject* _gameObject)
	: CollisionEventListenerBase(_gameObject)
{
}

void LaserCollisionEventListener::OnCollisionEnter(ColliderBase* _other)
{
	Laser* _laser = dynamic_cast<Laser*>(m_GameObject->GetComponent<Weapon>());
	if (_laser == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_laser->Attack(_miner);
}

void LaserCollisionEventListener::OnCollisionStay(ColliderBase* _other)
{
	Laser* _laser = dynamic_cast<Laser*>(m_GameObject->GetComponent<Weapon>());
	if (_laser == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_laser->Attack(_miner);
}

void LaserCollisionEventListener::OnCollisionExit(ColliderBase* _other)
{
}
