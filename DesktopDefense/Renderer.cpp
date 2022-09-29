#include "Renderer.h"

Engine::Renderer::Renderer(GameObject* _gameObject)
	: ComponentBase(_gameObject)
{
}

void Renderer::Tick()
{
	ComponentBase::Tick();

	Screen::GetInstance()->PrintSprite(m_Sprite, m_GameObject->m_Position);
}
