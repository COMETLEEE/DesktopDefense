#include "MiniGunCollisionEventListener.h"

#include "MiniGun.h"
#include "Time.h"
#include "Weapon.h"

MiniGunCollisionEventListener::MiniGunCollisionEventListener(GameObject* _gameObject)
	: CollisionEventListenerBase(_gameObject)
{
}

void MiniGunCollisionEventListener::OnCollisionEnter(ColliderBase* _other)
{
	MiniGun* _miniGun = dynamic_cast<MiniGun*>(m_GameObject->GetComponent<Weapon>());
	if (_miniGun == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_miniGun->Attack(_miner);
}

void MiniGunCollisionEventListener::OnCollisionStay(ColliderBase* _other)
{
	MiniGun* _miniGun = dynamic_cast<MiniGun*>(m_GameObject->GetComponent<Weapon>());
	if (_miniGun == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_miniGun->Attack(_miner);
}

void MiniGunCollisionEventListener::OnCollisionExit(ColliderBase* _other)
{
}