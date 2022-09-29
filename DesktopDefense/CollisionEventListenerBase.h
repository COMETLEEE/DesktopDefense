#pragma once
#include "ColliderBase.h"
#include "ComponentBase.h"

namespace Engine
{
	class CollisionEventListenerBase : public ComponentBase
	{
	public:
		explicit CollisionEventListenerBase(GameObject* _gameObject);

		virtual void OnCollisionEnter(ColliderBase* _other) = 0;

		virtual void OnCollisionStay(ColliderBase* _other) = 0;

		virtual void OnCollisionExit(ColliderBase* _other) = 0;

		// public:
		// 	friend void ColliderBase::OnCollisionEnter(ColliderBase* _other);
		//
		// 	friend void ColliderBase::OnCollisionStay(ColliderBase* _other);
		//
		// 	friend void ColliderBase::OnCollisionExit(ColliderBase* _other);
	};
}
