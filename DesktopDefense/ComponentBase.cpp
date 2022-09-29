#include "ComponentBase.h"

using namespace Engine;

ComponentBase::ComponentBase(GameObject* _gameObject)
	: m_GameObject(_gameObject),
	  m_bEnable(true),
	  m_Ticked(false)
{
}

void ComponentBase::PreTick()
{
	m_Ticked = false;
}

void ComponentBase::TryTick()
{
	if (m_bEnable == false)
		return;

	if (m_Ticked == true)
		return;

	Tick();
}

void ComponentBase::Tick()
{
	m_Ticked = true;
}

void ComponentBase::OnChangeEnable(bool _newEnable)
{
}
