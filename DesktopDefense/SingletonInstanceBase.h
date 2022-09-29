#pragma once
#include "ObjectBase.h"

namespace Engine
{
	template <class TClass>
	class SingletonInstanceBase : public ObjectBase<SingletonInstanceBase<TClass>>
	{
	public:
		SingletonInstanceBase();

		virtual ~SingletonInstanceBase();

		static TClass* GetInstance();

	protected:
		static TClass* Instance;
	};

	template <class TClass>
	SingletonInstanceBase<TClass>::SingletonInstanceBase()
	{
		// TODO: 이미 같은 타입의 인스턴스가 존재한다면 경고나 assert하자.

		Instance = static_cast<TClass*>(this);
	}

	template <class TClass>
	SingletonInstanceBase<TClass>::~SingletonInstanceBase()
	{
		Instance = nullptr;
	}

	template <class TClass>
	TClass* SingletonInstanceBase<TClass>::GetInstance()
	{
		return Instance;
	}

	template <class TClass>
	TClass* SingletonInstanceBase<TClass>::Instance = nullptr;
}
