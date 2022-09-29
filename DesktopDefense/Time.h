#pragma once

#include <ctime>

#include "SingletonInstanceBase.h"

namespace Engine
{
	class Time : public SingletonInstanceBase<Time>
	{
	public:
		Time();

		void PreTick();

		void PostTick();

	private:
		clock_t m_StartGameMillis;

		clock_t m_LastPreTickMillis;

		clock_t m_CurrentPreTickMillis;

	private:
		float m_GameTime;

		float m_LastDeltaTime;

		float m_LastProcessingTime;

	public:
		inline float GetGameTime() const { return m_GameTime; }

		inline float GetDeltaTime() const { return m_LastDeltaTime; }

		inline float GetProcessingTime() const { return m_LastProcessingTime; }
	};
}
