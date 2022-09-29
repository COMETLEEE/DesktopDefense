#pragma once
#include "CollisionEventListenerBase.h"

using namespace Engine;

class HammerCollisionEventListener : public CollisionEventListenerBase
{
public:
	HammerCollisionEventListener(GameObject* _gameObject);
	
	void OnCollisionEnter(ColliderBase* _other) override;

	void OnCollisionStay(ColliderBase* _other) override;

	void OnCollisionExit(ColliderBase* _other) override;
};
