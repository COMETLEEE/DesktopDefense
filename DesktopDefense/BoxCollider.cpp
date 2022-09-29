#include "BoxCollider.h"
#include "CircleCollider.h"
#include "GameObject.h"
#include <math.h>
#include <Windows.h>
using namespace Engine;

BoxCollider::BoxCollider(GameObject* _gameObject)
	:ColliderBase(_gameObject)
{
	this->m_Pivot = _gameObject->m_Position; // 일단 오브젝트 중심에다가 콜라이더 피봇 박음 .. (스프라이트로 해야하나 ..?)
}

Vector2D BoxCollider::GetCollidersPivot() const
{
	return this->m_Pivot;
}

int Engine::BoxCollider::GetWidth() const
{
	return this->m_Width;
}

int BoxCollider::GetHeight() const
{
	return this->m_Height;
}

bool BoxCollider::CheckCollide(ColliderBase* _other) const // Box Collider가 _source
{
	// 충돌한 콜라이더는 Circle 또는 Box 둘 중 하나입니다.
	CircleCollider* _isCircle = dynamic_cast<CircleCollider*>(_other);

	BoxCollider* _isBox = dynamic_cast<BoxCollider*>(_other);

	if (_isCircle != nullptr) // 충돌한 인스턴스는 Circle Collider이다. (Box vs Circle)
	{
		double _sourceX = this->m_Pivot.x;
		double _sourceY = this->m_Pivot.y;

		int _sourceWidth = this->m_Width;
		int _sourceHeight = this->m_Height;
		int _sourceWidthPlusRadius = _sourceWidth + 2 * _isCircle->GetRadius(); // 사각형의 크기를 원의 반지름 만큼 양 옆으로 확장
		int _sourceHeightPlusRadius = _sourceHeight + 2 * _isCircle->GetRadius(); // 사각형의 크기를 원의 반지름 만큼 위 아래로 확장 (원의 중심과 사각형의 변 간의 관계만 보기 위한 조정)

		Vector2D _othersPivot = _isCircle->GetCollidersPivot();
		double _otherX = _othersPivot.x;
		double _otherY = _othersPivot.y;

		if (_sourceX - _sourceWidthPlusRadius / 2 <= _otherX && // 원의 중심이 사각형의 왼쪽 변보다 오른쪽에 있으며
			_otherX <= _sourceX + _sourceWidthPlusRadius / 2 && // 사각형의 오른쪽 변보다 왼쪽에 있고
			_sourceY - _sourceHeightPlusRadius / 2 <= _otherY && // 사각형의 윗변보다 아랫쪽에 있으며
			_otherY <= _sourceY + _sourceHeightPlusRadius / 2) // 사각형의 아랫변보다 윗쪽에 있음, 즉, 원이 사각형의 안쪽에 있다. = 충돌했다. (Vertex 인근 작은 사각형 영역에 대한 예외처리 필요)
			return true;
		else if ((sqrt((_sourceX - _sourceWidth / 2 - _otherX) * (_sourceX - _sourceWidth / 2 - _otherX) +
				(_sourceY - _sourceHeight / 2 - _sourceY) * (_sourceY - _sourceHeight / 2 - _otherY)) <= _isCircle->GetRadius()) || // 사각형의 왼쪽 상단 꼭지점이 원 안에 있다
				(sqrt((_sourceX - _sourceWidth / 2 - _otherX) * (_sourceX - _sourceWidth / 2 - _otherX) +
				(_sourceY + _sourceHeight / 2 - _sourceY) * (_sourceY + _sourceHeight / 2 - _otherY)) <= _isCircle->GetRadius()) || // 사각형의 왼쪽 하단 꼭지점이 원 안에 있다
				(sqrt((_sourceX + _sourceWidth / 2 - _otherX) * (_sourceX + _sourceWidth / 2 - _otherX) +
				(_sourceY - _sourceHeight / 2 - _sourceY) * (_sourceY - _sourceHeight / 2 - _otherY)) <= _isCircle->GetRadius()) || // 사각형의 오른쪽 상단 꼭지점이 원 안에 있다.
				(sqrt((_sourceX + _sourceWidth / 2 - _otherX) * (_sourceX + _sourceWidth / 2 - _otherX) +
				(_sourceY + _sourceHeight / 2 - _sourceY) * (_sourceY + _sourceHeight / 2 - _otherY)) <= _isCircle->GetRadius())) // 사각형의 오른쪽 하단 꼭지점이 원 안에 있다. => 충돌했다.
			return true;
		else
			return false;
	}
	else if (_isBox != nullptr) // 충돌한 인스턴스는 Box Collider이다. (Box vs Box) => 4가지 경우 (사각형의 꼭지점이 다른 사각형의 내부에 있는가)
	{
		double _sourceX = this->m_Pivot.x;
		double _sourceY = this->m_Pivot.y;
		int _sourceWidth = this->m_Width;
		int _sourceHeight = this->m_Height;
	
		Vector2D _othersPivot = _isBox->GetCollidersPivot();
		double _otherX = _othersPivot.x;
		double _otherY = _othersPivot.y;
		int _otherWidth = _isBox->GetWidth();
		int _otherHeight = _isBox->GetHeight();

		// 나중에 min, max (Window 사이즈 걸어줘야함 .. ! => 화면 범위를 넘어갔을 때 윈도우 창의 끝과 끝으로 사각형 표현)
		RECT _sourceRect = { _sourceX - _sourceWidth / 2, _sourceY - _sourceHeight, _sourceX + _sourceWidth / 2, _sourceY + _sourceHeight / 2 };
		RECT _otherRect = { _otherX - _otherWidth / 2, _otherY -_otherHeight / 2, _otherX + _otherWidth / 2, _otherY + _otherHeight };
		RECT _temp;

		if (IntersectRect(&_temp, &_sourceRect, &_otherRect)) // Box Collider끼리 교차하고 있으면 0이 아닌 값을 반환합니다.
			return true;
		else
			return false;
	}
	else // 예외상황 ! false를 던진다.
		return false;
}