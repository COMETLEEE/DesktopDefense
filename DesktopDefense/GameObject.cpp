#include "GameObject.h"

#include "ComponentBase.h"
#include "GameInstanceBase.h"
#include "GameWorld.h"

using namespace Engine;

GameObject::GameObject()
	: m_bEnable(true),
	  m_Position(Vector2D::Zero)
{
	GameWorld::GetInstance()->AddGameObject(this);
}

GameObject::~GameObject()
{
	//GameObject가 소멸될때 GameObject가 가지고 있는 component가 전부 삭제됨
	for (ComponentBase* _component : this->m_Components)
	{
		delete _component;
	}
}

void GameObject::PreTick()
{
	for (const auto& _component : m_Components)
		_component->PreTick();
}

void GameObject::Tick()
{
	for (const auto& _component : m_Components)
		_component->TryTick();
}

void GameObject::SetEnable(bool _value)
{
	if (_value == m_bEnable)
		return;

	m_bEnable = _value;

	GameWorld::GetInstance()->UpdateGameObjectEnable(this);
}
