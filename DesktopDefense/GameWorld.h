#pragma once
#include <set>

#include "ColliderBase.h"
#include "SingletonInstanceBase.h"
#include <vector>

#include "CollisionManager.h"

namespace Engine
{
	class GameWorld : public SingletonInstanceBase<GameWorld>
	{
	public:
		GameWorld(class GameInstanceBase* _gameInstance);

		~GameWorld() override;

		void Initialize();

		void PreTick();

		void PostTick();

		void Tick();

		void Destroy(GameObject* _gameObject);

		void ReleaseGameObjectsRegisteredForDestroy();

		void UpdateGameObjectEnable(GameObject* _gameObject);

		void DisableGameObjectsRegisteredForUpdateEnable();

		void AddGameObject(GameObject* _gameObject);

		void RemoveGameObject(GameObject* _gameObject);

	private:
		class GameInstanceBase* m_GameInstance;

		std::set<class GameObject*> m_GameObjects;

		std::set<class GameObject*> m_GameObjectsDisabled;

		std::set<class GameObject*> m_GameObjectsWillDestroy;

		std::set<class GameObject*> m_GameObjectsWillUpdateEnable;

		std::set<class ColliderBase*> m_Colliders;

		std::set<class Renderer*> m_Renderers;

		CollisionManager* m_CollisionManager;

	public:
		inline void AddCollider(class ColliderBase* _collider) { m_Colliders.insert(_collider); }

		inline void RemoveCollider(class ColliderBase* _collider) { m_Colliders.erase(m_Colliders.find(_collider)); }

		inline void AddRenderer(class Renderer* _renderer) { m_Renderers.insert(_renderer); }

		inline void RemoveRenderer(class Renderer* _renderer) { m_Renderers.erase(m_Renderers.find(_renderer)); }
	};
}
