#pragma once
#include "ComponentBase.h"
#include "Vector2D.h"

namespace Engine
{
	class Button : public ComponentBase
	{
	public: 
		explicit Button(GameObject* _gameObject);
		bool OnClick(Vector2D currentmouseposition);
	//private:
		Vector2D m_StartPosition;
		Vector2D m_EndPosition;
	};
}