#pragma once
#include "CollisionEventListenerBase.h"

using namespace Engine;

class MiniGunCollisionEventListener : public CollisionEventListenerBase
{
public:
	explicit MiniGunCollisionEventListener(GameObject* _gameObject);

	void OnCollisionEnter(ColliderBase* _other) override;

	void OnCollisionStay(ColliderBase* _other) override;

	void OnCollisionExit(ColliderBase* _other) override;
};
