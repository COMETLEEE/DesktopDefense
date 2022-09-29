#include "Button.h"
#include "GameObject.h"

using namespace Engine;

Button::Button(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
}

bool Button::OnClick(Vector2D currentmouseposition)
{
	if (m_StartPosition.x <= currentmouseposition.x && currentmouseposition.x <= m_EndPosition.x &&
		m_StartPosition.y <= currentmouseposition.y && currentmouseposition.y <= m_EndPosition.y)
		return true;
	else
		return false;
}