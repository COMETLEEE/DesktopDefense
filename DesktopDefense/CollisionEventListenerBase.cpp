#include "CollisionEventListenerBase.h"

Engine::CollisionEventListenerBase::CollisionEventListenerBase(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
}

// 박스 안에 마이너가 들어가면 죽인다 ..?
// 