#include "CircleCollider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include <math.h>
#include <Windows.h>
using namespace Engine;

CircleCollider::CircleCollider(GameObject* _gameObject)
	:ColliderBase(_gameObject)
{
	this->m_Pivot = _gameObject->m_Position;  // 일단 오브젝트 중심에다가 콜라이더 피봇 박음 .. (스프라이트로 해야하나 ..?)
}

Vector2D CircleCollider::GetCollidersPivot() const
{
	return this->m_Pivot;
}

int CircleCollider::GetRadius() const
{
	return this->m_Radius;
}

bool CircleCollider::CheckCollide(ColliderBase* _other) const
{
	// 충돌한 콜라이더는 Circle 또는 Box 둘 중 하나입니다.
	CircleCollider* _isCircle =	dynamic_cast<CircleCollider*>(_other);

	BoxCollider* _isBox = dynamic_cast<BoxCollider*>(_other);

	if (_isCircle != nullptr) // 충돌한 인스턴스는 Circle Collider이다. (Circle vs Circle)
	{
		/*double _sourceX = this->m_Pivot.x;
		double _sourceY = this->m_Pivot.y;*/

		double _sourceX = this->m_GameObject->m_Position.x;
		double _sourceY = this->m_GameObject->m_Position.y;

		Vector2D _othersPivot = _isCircle->GetCollidersPivot();
		/*double _otherX = _othersPivot.x;
		double _otherY = _othersPivot.y;*/

		double _otherX = _other->m_GameObject->m_Position.x;
		double _otherY = _other->m_GameObject->m_Position.y;

		if ((this->m_Radius + _isCircle->GetRadius()) * (this->m_Radius + _isCircle->GetRadius())
			>= (_sourceX - _otherX) * (_sourceX - _otherX) + (_sourceY - _otherY) * (_sourceY - _otherY))
			return true;
		else
			return false;
	}
	else if (_isBox != nullptr) // 충돌한 인스턴스는 Box Collider이다. (Circle vs Box)
	{
		double _sourceX = this->m_Pivot.x;
		double _sourceY = this->m_Pivot.y;

		Vector2D _othersPivot = _isBox->GetCollidersPivot();
		double _otherX = _othersPivot.x;
		double _otherY = _othersPivot.y;

		int _otherWidth = _isBox->GetWidth();
		int _otherHeight = _isBox->GetHeight();
		int _otherWidthPlusRadius = _otherWidth + 2 * this->GetRadius(); // 사각형의 크기를 원의 반지름 만큼 양 옆으로 확장
		int _otherHeightPlusRadius = _otherHeight + 2 * this->GetRadius(); // 사각형의 크기를 원의 반지름 만큼 위 아래로 확장 (원의 중심과 사각형의 변 간의 관계만 보기 위한 조정)

		if (_otherX - _otherWidthPlusRadius / 2 <= _sourceX && // 원의 중심이 사각형의 왼쪽 변보다 오른쪽에 있으며
			_sourceX <= _otherX + _otherWidthPlusRadius / 2 && // 사각형의 오른쪽 변보다 왼쪽에 있으며
			_otherY - _otherHeightPlusRadius / 2 <= _sourceY && // 사각형의 윗변보다 아랫쪽에 있으며
			_sourceY <= _otherY + _otherHeightPlusRadius / 2) // 사각형의 아랫변보다 윗쪽에 있음, 즉, 그림을 그려보면 원이 사각형의 안쪽에 있다. = 충돌했다. (Vectex 예외처리 필요)
			return true;
		else if ((sqrt((_otherX - _otherWidth / 2 - _sourceX) * (_otherX - _otherWidth / 2 - _sourceX) +
				(_otherY - _otherHeight / 2 - _sourceY) * (_otherY - _otherHeight / 2 - _sourceY)) <= this->m_Radius) || // 사각형의 왼쪽 상단 꼭지점이 원 안에 있다
				(sqrt((_otherX - _otherWidth / 2 - _sourceX) * (_otherX - _otherWidth / 2 - _sourceX) +
				(_otherY + _otherHeight / 2 - _sourceY) * (_otherY + _otherHeight / 2 - _sourceY)) <= this->m_Radius) || // 사각형의 왼쪽 하단 꼭지점이 원 안에 있다
				(sqrt((_otherX + _otherWidth / 2 - _sourceX) * (_otherX + _otherWidth / 2 - _sourceX) +
				(_otherY - _otherHeight / 2 - _sourceY) * (_otherY - _otherHeight / 2 - _sourceY)) <= this->m_Radius) || // 사각형의 오른쪽 상단 꼭지점이 원 안에 있다.
				(sqrt((_otherX + _otherWidth / 2 - _sourceX) * (_otherX + _otherWidth / 2 - _sourceX) +
				(_otherY + _otherHeight / 2 - _sourceY) * (_otherY + _otherHeight / 2 - _sourceY)) <= this->m_Radius)) // 사각형의 오른쪽 하단 꼭지점이 원 안에 있다. => 충돌했다.
			return true;
		else
			return false;
	}
	else // 예외상황 ! false를 던진다.
		return false;
}