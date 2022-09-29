#pragma once

#include "ColliderBase.h"
#include "Vector2D.h"

namespace Engine
{
	class CircleCollider : public ColliderBase
	{
	public:
		explicit CircleCollider(GameObject* _gameObject);

		bool CheckCollide(ColliderBase* _other) const override;
		Vector2D GetCollidersPivot() const;
		int GetRadius() const;
	/*private:*/
		int m_Radius;
		Vector2D m_Pivot; // 스프라이트의 Pivot이랑 일치하게 될 듯
	};
}