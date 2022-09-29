#pragma once

#include <unordered_map>
#include <unordered_set>

#include "GameObject.h"
#include "SingletonInstanceBase.h"
#include "Vector2D.h"

namespace Engine
{
	class CollisionManager : public SingletonInstanceBase<CollisionManager>
	{
		class Cell
		{
		private:
			Vector2D m_Position;

			std::unordered_set<GameObject*> m_GameObjectSet;

		public:
			inline void AddGameObject(GameObject* _gameObject) { m_GameObjectSet.insert(_gameObject); }

			inline void RemoveGameObject(GameObject* _gameObject) { m_GameObjectSet.erase(_gameObject); }

			inline std::unordered_set<GameObject*> GetGameObjects() const { return m_GameObjectSet; }
		};

	private:
		/// <summary>
		/// 하나의 Cell의 가로 및 세로의 크기입니다.
		/// </summary>
		static constexpr int CELL_WIDTH = 50;

		/// <summary>
		/// 모든 Cell들의 레퍼런스를 가지는 Map입니다.
		/// </summary>
		std::unordered_map<Vector2D, Cell> m_CellMap;

		std::unordered_set<GameObject*> m_GameObjectSet;

		std::unordered_map<GameObject*, std::unordered_set<GameObject*>> m_CollideState;

	public:
		void CollisionTick();

		void AddGameObject(GameObject* _gameObject);

		void RemoveGameObject(GameObject* _gameObject);

		/// <summary>
		/// 게임 오브젝트의 충돌 범위에 포함되는 Cell들에 게임 오브젝트를 추가합니다.
		///	범위를 포함하는 Cell이 없다면 새로 생성합니다.
		/// </summary>
		void AddGameObjectToProperCell(GameObject* _gameObject);

		/// <summary>
		/// ColliderMap에서 해당 게임 오브젝트의 정보를 모두 삭제합니다.
		/// </summary>
		/// <param name="_gameObject"></param>
		void RemoveGameObjectFromProperCell(GameObject* _gameObject);

		/// <summary>
		/// 게임 오브젝트의 충돌 범위에 포함되는 Cell의 위치 영역을 반환합니다.
		/// </summary>
		void GetGameObjectRange(GameObject* _gameObject, Vector2D& _outCellMinPosition, Vector2D& _outCellMaxPosition) const;

		/// <summary>
		/// 게임 오브젝트의 충돌 범위에 포함되는 Cell들의 위치 영역 안에 있는 모든 게임 오브젝트들을 반환합니다.
		/// </summary>
		/// <param name="_gameObject"></param>
		/// <returns></returns>
		std::unordered_set<GameObject*> GetGameObjectsInRange(GameObject* _gameObject) const;
	};
}
