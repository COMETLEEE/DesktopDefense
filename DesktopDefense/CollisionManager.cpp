#include "CollisionManager.h"

#include "BoxCollider.h"
#include "CircleCollider.h"
#include "GameObject.h"

using namespace Engine;

void CollisionManager::CollisionTick()
{
	// CellMap을 초기화합니다.
	m_CellMap = std::unordered_map<Vector2D, Cell>();
	for (const auto& _gameObject : m_GameObjectSet)
	{
		AddGameObjectToProperCell(_gameObject);
	}
}

void CollisionManager::AddGameObject(GameObject* _gameObject)
{
	if (_gameObject == nullptr)
		return;

	m_GameObjectSet.emplace(_gameObject);
}

void CollisionManager::RemoveGameObject(GameObject* _gameObject)
{
	if (_gameObject == nullptr)
		return;

	m_GameObjectSet.erase(_gameObject);
}

void CollisionManager::AddGameObjectToProperCell(GameObject* _gameObject)
{
	if (_gameObject == nullptr)
		return;

	const int _cellPositionX = static_cast<int>(_gameObject->m_Position.x / CELL_WIDTH);
	const int _cellPositionY = static_cast<int>(_gameObject->m_Position.y / CELL_WIDTH);
	const Vector2D _cellPosition{static_cast<double>(_cellPositionX), static_cast<double>(_cellPositionY)};

	//// CellMap에 관리되고 있지 않았던 Cell이라면 추가합니다.
	if (m_CellMap.find(_cellPosition) == m_CellMap.end())
		m_CellMap.insert({_cellPosition, Cell{}});

	m_CellMap[_cellPosition].AddGameObject(_gameObject);
}

void CollisionManager::RemoveGameObjectFromProperCell(GameObject* _gameObject)
{
	if (_gameObject == nullptr)
		return;

	const int _cellPositionX = static_cast<int>(_gameObject->m_Position.x / CELL_WIDTH);
	const int _cellPositionY = static_cast<int>(_gameObject->m_Position.y / CELL_WIDTH);
	const Vector2D _cellPosition{static_cast<double>(_cellPositionX), static_cast<double>(_cellPositionY)};

	// CellMap에 관리되고 있지 않았던 Cell이라면 추가합니다.
	if (m_CellMap.find(_cellPosition) == m_CellMap.end())
		return;

	m_CellMap[_cellPosition].RemoveGameObject(_gameObject);
}

void CollisionManager::GetGameObjectRange(GameObject* _gameObject, Vector2D& _outCellMinPosition, Vector2D& _outCellMaxPosition) const
{
	if (_gameObject == nullptr)
		return;

	ColliderBase* _collider = _gameObject->GetComponent<ColliderBase>();
	if (_collider == nullptr)
		return;

	double _minX = 0, _maxX = 0;
	double _minY = 0, _maxY = 0;

	BoxCollider* _boxCollider = dynamic_cast<BoxCollider*>(_collider);
	if (_boxCollider != nullptr)
	{
		int _halfWidth = _boxCollider->m_Width / 2;
		int _halfHeight = _boxCollider->m_Height / 2;

		_minX = _gameObject->m_Position.x - _halfWidth;
		_maxX = _gameObject->m_Position.x + _halfWidth;

		_minY = _gameObject->m_Position.y - _halfHeight;
		_maxY = _gameObject->m_Position.y + _halfHeight;
	}

	CircleCollider* _circleCollider = dynamic_cast<CircleCollider*>(_collider);
	if (_circleCollider != nullptr)
	{
		int _halfRadius = _circleCollider->m_Radius / 2;

		_minX = _gameObject->m_Position.x - _halfRadius;
		_maxX = _gameObject->m_Position.x + _halfRadius;

		_minY = _gameObject->m_Position.y - _halfRadius;
		_maxY = _gameObject->m_Position.y + _halfRadius;
	}

	const int _cellMinX = static_cast<int>(_minX / CELL_WIDTH);
	const int _cellMaxX = static_cast<int>(_maxX / CELL_WIDTH);

	const int _cellMinY = static_cast<int>(_minY / CELL_WIDTH);
	const int _cellMaxY = static_cast<int>(_maxY / CELL_WIDTH);

	_outCellMinPosition.x = _cellMinX;
	_outCellMinPosition.y = _cellMinY;

	_outCellMaxPosition.x = _cellMaxX;
	_outCellMaxPosition.y = _cellMaxY;
}

std::unordered_set<GameObject*> CollisionManager::GetGameObjectsInRange(GameObject* _gameObject) const
{
	if (_gameObject == nullptr)
		return std::unordered_set<GameObject*>();

	Vector2D _cellMinPosition;
	Vector2D _cellMaxPosition;
	GetGameObjectRange(_gameObject, _cellMinPosition, _cellMaxPosition);

	const int _cellMinPositionX = static_cast<int>(_cellMinPosition.x);
	const int _cellMaxPositionX = static_cast<int>(_cellMinPosition.x);
	const int _cellMinPositionY = static_cast<int>(_cellMinPosition.x);
	const int _cellMaxPositionY = static_cast<int>(_cellMinPosition.x);

	std::unordered_set<GameObject*> _outSet;
	for (int _y = _cellMinPositionY; _y <= _cellMaxPositionY; ++_y)
	{
		for (int _x = _cellMinPositionX; _x <= _cellMaxPositionX; ++_x)
		{
			const Vector2D _cellPosition{(double)_x, (double)_y};
			const Cell& _cell = m_CellMap.find(_cellPosition) != m_CellMap.end() ? m_CellMap.at(_cellPosition) : Cell();

			auto _gameObjectsInCell = _cell.GetGameObjects();

			for (const auto& _go : _gameObjectsInCell)
			{
				_outSet.insert(_go);
			}
		}
	}

	return _outSet;
}
