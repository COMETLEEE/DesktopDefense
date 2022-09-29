#include "Weapon.h"

#include "CircleCollider.h"
#include "Vector2D.h"

Weapon::Weapon(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
	this->m_LastAttackTime = Time::GetInstance()->GetGameTime();
	this->m_TimePerAttack = Time::GetInstance()->GetGameTime();

	auto _collider = m_GameObject->AddComponent<CircleCollider>();
	_collider->m_Radius = 40;
	_collider->SetEnable(false);

	Vector2D m_AttackMousePos;
};