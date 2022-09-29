#pragma once
#include <vector>

#include "ObjectBase.h"
#include "Vector2D.h"

namespace Engine
{
	class GameObject : public ObjectBase<GameObject>
	{
	public:
		GameObject();

		~GameObject();

		/// <summary>
		/// 매 프레임마다 World의 모든 오브젝트의 Tick()이 호출되기 전에 호출되는 메소드입니다.
		///	일반적으로 매 프레임마다 처리되기 앞서 초기화되어야 할 처리를 담당합니다.
		/// </summary>
		void PreTick();

		/// <summary>
		/// 매 프레임마다 World에 의해 호출되는 메소드입니다.
		/// </summary>
		virtual void Tick();

		/// <summary>
		/// 게임 컴포넌트가 가지고 있는 컴포넌트 중에서 입력받은 타입과 일치하거나 이 타입의 서브타입의 컴포넌트가 있는 경우
		///	해당 컴포넌트를 반환합니다.
		///	만약 해당하는 컴포넌트가 없다면 nullptr을 반화니합니다.
		/// </summary>
		template <class TComponent>
		TComponent* GetComponent();

		template <class TComponent>
		TComponent* AddComponent();

	public:
		Vector2D m_Position;

	private:
		bool m_bEnable;

		std::vector<class ComponentBase*> m_Components;

	public:
		inline bool GetEnable() const { return m_bEnable; }

		void SetEnable(bool _value);
	};

	template <class TComponent>
	TComponent* GameObject::GetComponent()
	{
		for (auto _component : m_Components)
		{
			TComponent* _castedComponent = dynamic_cast<TComponent*>(_component);

			if (_castedComponent != nullptr)
				return _castedComponent;
		}

		return nullptr;
	}

	template <class TComponent>
	TComponent* GameObject::AddComponent()
	{
		static_assert(std::is_base_of<ComponentBase, TComponent>::value, "TComponent must inherit from ComponentBase");

		TComponent* _newComponent = new TComponent(this);

		m_Components.emplace_back(_newComponent);
		return _newComponent;
	}
}
