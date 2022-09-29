#include "Time.h"

using namespace Engine;

Time::Time()
	: SingletonInstanceBase<Engine::Time>(),
	  m_StartGameMillis{clock()},
	  m_LastPreTickMillis{clock()},
	  m_CurrentPreTickMillis{clock()},
	  m_GameTime{0},
	  m_LastDeltaTime{0},
	  m_LastProcessingTime{0}
{
}

void Time::PreTick()
{
	m_LastPreTickMillis = m_CurrentPreTickMillis;
	m_CurrentPreTickMillis = clock();

	m_LastDeltaTime = static_cast<float>(m_CurrentPreTickMillis - m_LastPreTickMillis) / CLOCKS_PER_SEC;

	m_GameTime = static_cast<float>(m_CurrentPreTickMillis - m_StartGameMillis) / 1000.f;
}

void Time::PostTick()
{
	m_LastProcessingTime = static_cast<float>(clock() - m_CurrentPreTickMillis) / 1000.f;
}
