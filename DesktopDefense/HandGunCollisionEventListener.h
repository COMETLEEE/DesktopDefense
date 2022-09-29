#pragma once
#include "CollisionEventListenerBase.h"

using namespace Engine;

class HandGunCollisionEventListener : public CollisionEventListenerBase
{
public:
	explicit HandGunCollisionEventListener(GameObject* _gameObject);

	void OnCollisionEnter(ColliderBase* _other) override;

	void OnCollisionStay(ColliderBase* _other) override;

	void OnCollisionExit(ColliderBase* _other) override;
};
