#pragma once

#include "DesktopDefense.h"
#include "ComponentBase.h"

namespace Engine
{
	class Renderer : public ComponentBase
	{
	public:
		explicit Renderer(GameObject* _gameObject);

		void Tick() override;

	private:
		Sprite* m_Sprite;

	public:
		inline void SetSprite(Sprite* _sprite) { m_Sprite = _sprite; }
	};
}
