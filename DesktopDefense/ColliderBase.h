#pragma once
#include <unordered_set>

#include "ComponentBase.h"

namespace Engine
{
	class ColliderBase : public ComponentBase
	{
	public:
		explicit ColliderBase(GameObject* _gameObject);

		~ColliderBase();

		void Tick() override;

		void OnChangeEnable(bool _newEnable) override;

		virtual bool CheckCollide(ColliderBase* _other) const;

		void OnCollisionEnter(ColliderBase* _other);

		void OnCollisionStay(ColliderBase* _other);

		void OnCollisionExit(ColliderBase* _other);

	protected:
		std::unordered_set<GameObject*> m_CollidedObjects;

	public:
		inline void AddCollidedObject(GameObject* _gameObject) { m_CollidedObjects.insert(_gameObject); }

		inline bool IsCollideWith(GameObject* _gameObject) const { return m_CollidedObjects.find(_gameObject) != m_CollidedObjects.end(); }
	};
}
