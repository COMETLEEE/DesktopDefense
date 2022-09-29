#include "ColliderBase.h"

#include "CollisionEventListenerBase.h"
#include "CollisionManager.h"

using namespace Engine;

ColliderBase::ColliderBase(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
	CollisionManager::GetInstance()->AddGameObject(_gameObject);
}

ColliderBase::~ColliderBase()
{
	CollisionManager::GetInstance()->RemoveGameObject(m_GameObject);
}

void ColliderBase::Tick()
{
	ComponentBase::Tick();

	auto _collidedObjectsLastTick = m_CollidedObjects;
	m_CollidedObjects.clear();

	// 이 게임오브젝트의 충돌 영역을 포함하는 Cell들 안에 있는 게임 오브젝트들의 목록을 가져옵니다.
	auto _gameObjectsInRange = CollisionManager::GetInstance()->GetGameObjectsInRange(m_GameObject);
	_gameObjectsInRange.erase(m_GameObject);

	// 영역 안의 게임 오브젝트들을 순회하며 실제로 충돌하는지 계산하고,
	// 실제 충돌이 일어난 오브젝트 양쪽에게 적절한 충돌 이벤트를 전송합니다.
	for (const auto& _other : _gameObjectsInRange)
	{
		ColliderBase* _otherCollider = _other->GetComponent<ColliderBase>();
		if (_otherCollider == nullptr)
			continue;

		// 이번 프레임에 이미 서로간의 충돌이 일어난 오브젝트들인 경우,
		// 충돌 검사를 다시 실시하지 않습니다.
		if (IsCollideWith(_other) == true)
			continue;

		// 이번 프레임에서 이 두 오브젝트간 충돌이 있는지에 대한 여부입니다.
		const bool _bCollision = CheckCollide(_otherCollider);

		// 지난 Tick에서도 이 두 오브젝트간 충돌이 있었는지에 대한 여부입니다.
		const bool _bCollideLastTick = _collidedObjectsLastTick.find(_other) != _collidedObjectsLastTick.end();

		if (_bCollision == true && _bCollideLastTick == false)
		{
			// 이번 Tick에서부터 충돌이 시작되었을 때 실행됩니다.
			OnCollisionEnter(_otherCollider);
			_otherCollider->OnCollisionEnter(this);
		}
		else if (_bCollision == true && _bCollideLastTick == true)
		{
			// 지난 Tick에서부터 충돌이 유지되었을 때 실행됩니다.
			OnCollisionStay(_otherCollider);
			_otherCollider->OnCollisionStay(this);
		}
		else if (_bCollision == false && _bCollideLastTick == true)
		{
			// 이번 Tick에서부터 충돌이 종료되었을 때 실행됩니다.
			OnCollisionExit(_otherCollider);
			_otherCollider->OnCollisionExit(this);
		}

		// 충돌이 일어났다는 상태를 서로의 충돌체 component에 기록합니다.
		if (_bCollision == true)
		{
			AddCollidedObject(_other);
			_otherCollider->AddCollidedObject(m_GameObject);
		}
	}
}

void ColliderBase::OnChangeEnable(bool _newEnable)
{
	if (_newEnable == false)
		m_CollidedObjects.clear();
}

bool ColliderBase::CheckCollide(ColliderBase* _other) const // 그냥 인터페이스
{
	return false;
}

void ColliderBase::OnCollisionEnter(ColliderBase* _other)
{
	auto _eventListener = m_GameObject->GetComponent<CollisionEventListenerBase>();

	if (_eventListener != nullptr)
		_eventListener->OnCollisionEnter(_other);
}

void ColliderBase::OnCollisionStay(ColliderBase* _other)
{
	auto _eventListener = m_GameObject->GetComponent<CollisionEventListenerBase>();

	if (_eventListener != nullptr)
		_eventListener->OnCollisionStay(_other);
}

void ColliderBase::OnCollisionExit(ColliderBase* _other)
{
	auto _eventListener = m_GameObject->GetComponent<CollisionEventListenerBase>();

	if (_eventListener != nullptr)
		_eventListener->OnCollisionExit(_other);
}
