#include "GameWorld.h"

#include "GameObject.h"

using namespace Engine;

GameWorld::GameWorld(GameInstanceBase* _gameInstance)
	: SingletonInstanceBase<Engine::GameWorld>(),
	  m_GameInstance(_gameInstance),
	  m_GameObjects(std::set<GameObject*>()),
	  m_Colliders(std::set<ColliderBase*>())
{
	m_CollisionManager = new CollisionManager();
}

GameWorld::~GameWorld()
{
	for (const auto& _gameObject : m_GameObjects)
		delete _gameObject;

	delete m_CollisionManager;
}

void GameWorld::Destroy(GameObject* _gameObject)
{
	m_GameObjectsWillDestroy.insert(_gameObject);
}

void GameWorld::ReleaseGameObjectsRegisteredForDestroy()
{
	for (auto _gameObject : m_GameObjectsWillDestroy)
	{
		RemoveGameObject(_gameObject);

		delete _gameObject;
	}

	m_GameObjectsWillDestroy.clear();
}

void GameWorld::UpdateGameObjectEnable(GameObject* _gameObject)
{
	m_GameObjectsWillUpdateEnable.insert(_gameObject);
}

void GameWorld::DisableGameObjectsRegisteredForUpdateEnable()
{
	for (auto _gameObject : m_GameObjectsWillUpdateEnable)
	{
		m_GameObjectsDisabled.erase(_gameObject);
		m_GameObjects.erase(_gameObject);

		if (_gameObject->GetEnable() == true)
		{
			m_GameObjects.insert(_gameObject);
		}
		else
		{
			m_GameObjectsDisabled.insert(_gameObject);
		}
	}

	m_GameObjectsWillUpdateEnable.clear();
}

void GameWorld::AddGameObject(GameObject* _gameObject)
{
	if (_gameObject->GetEnable() == true)
		m_GameObjects.insert(_gameObject);
	else
		m_GameObjectsDisabled.insert(_gameObject);
}

void GameWorld::RemoveGameObject(GameObject* _gameObject)
{
	m_GameObjectsWillUpdateEnable.erase(_gameObject);

	if (_gameObject->GetEnable() == true)
	{
		m_GameObjects.erase(_gameObject);
	}
	else
	{
		m_GameObjectsDisabled.erase(_gameObject);
	}
}

void GameWorld::Initialize()
{
}

void GameWorld::PreTick()
{
	for (auto _iter = m_GameObjects.begin(); _iter != m_GameObjects.end(); _iter++)
	{
		GameObject* _gameObject = *_iter;
		_gameObject->PreTick();
	}
}

void GameWorld::PostTick()
{
	ReleaseGameObjectsRegisteredForDestroy();

	DisableGameObjectsRegisteredForUpdateEnable();
}

void GameWorld::Tick()
{
	// Collision 검사를 실시합니다.
	m_CollisionManager->CollisionTick();

	for (auto _iter = m_GameObjects.begin(); _iter != m_GameObjects.end(); _iter++)
	{
		GameObject* _gameObject = *_iter;
		_gameObject->Tick();
	}
}
