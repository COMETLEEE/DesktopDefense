#include "CursorCollisionEventListener.h"

#include "Cursor.h"
#include "Time.h"
#include "Weapon.h"

CursorCollisionEventListener::CursorCollisionEventListener(GameObject* _gameObject)
	: CollisionEventListenerBase(_gameObject)
{
}

void CursorCollisionEventListener::OnCollisionEnter(ColliderBase* _other)
{
	Cursor* _cursor = dynamic_cast<Cursor*>(m_GameObject->GetComponent<Weapon>());
	if (_cursor == nullptr)
		return;

	Miner* _miner = _other->m_GameObject->GetComponent<Miner>();
	if (_miner == nullptr)
		return;

	_cursor->Attack(_miner);
}

void CursorCollisionEventListener::OnCollisionStay(ColliderBase* _other)
{
}

void CursorCollisionEventListener::OnCollisionExit(ColliderBase* _other)
{
}
