#pragma once

#include "ColliderBase.h"
#include "Vector2D.h"

namespace Engine
{
	class BoxCollider : public ColliderBase
	{
	public:
		BoxCollider(GameObject* _gameObject);
		bool CheckCollide(ColliderBase* _other) const override;
		Vector2D GetCollidersPivot() const;
		int GetWidth() const;
		int GetHeight() const;
		/*private:*/
		int m_Width;
		int m_Height;
		Vector2D m_Pivot; // 스프라이트의 Pivot이랑 일치하게 될 듯
	};
};