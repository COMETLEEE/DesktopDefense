#include "Vector2D.h"

#include "DesktopDefense.h"

using namespace Engine;

const Vector2D Vector2D::Left = {-1, 0};
const Vector2D Vector2D::Right = {1, 0};
const Vector2D Vector2D::Up = {0, 1};
const Vector2D Vector2D::Down = {0, -1};
const Vector2D Vector2D::RightUp = {1, 1};
const Vector2D Vector2D::RightDown = {1, -1};
const Vector2D Vector2D::LeftUp = {-1, 1};
const Vector2D Vector2D::LeftDown = {-1, -1};
const Vector2D Vector2D::Zero = {0, 0};
const Vector2D Vector2D::Center = {.5, .5};
const Vector2D Vector2D::One = {1, 1};

Vector2D::Vector2D(double _x, double _y) :
	x{_x},
	y{_y}
{
}

Vector2D Vector2D::operator+(Vector2D _other) const
{
	return {x + _other.x, y + _other.y};
}

Vector2D Vector2D::operator-(Vector2D _other) const
{
	return {x - _other.x, y - _other.y};
}

Vector2D Vector2D::operator*(double _scalar) const
{
	return {x * _scalar, y * _scalar};
}

Vector2D Vector2D::operator/(double _scalar) const
{
	return {x / _scalar, y / _scalar};
}

bool Vector2D::operator==(const Vector2D& _other) const
{
	const bool _isXEquals = abs(x - _other.x) <= EPSILON;
	const bool _isYEquals = abs(y - _other.y) <= EPSILON;

	return _isXEquals && _isYEquals;
}

bool Vector2D::operator!=(const Vector2D& _other) const
{
	return !(*this == _other);
}

Vector2D Vector2D::Normalize() const
{
	if (*this == Vector2D::Zero)
		return Vector2D::Zero;

	return *this / Magnitude();
}

double Vector2D::Magnitude() const
{
	if (*this == Vector2D::Zero)
		return 0.;

	const double _powX = x * x;
	const double _powY = y * y;
	const double _outMagnitude = sqrt(_powX + _powY);
	return _outMagnitude;
}

double Vector2D::DistanceTo(Vector2D& _other) const
{
	const Vector2D _difference = {
		abs(abs(x) - abs(_other.x)),
		abs(abs(y) - abs(_other.y))
	};

	return _difference.Magnitude();
}

Vector2D Vector2D::Lerp(Vector2D _origin, Vector2D _destination, double _delta)
{
	const Vector2D _direction = _destination - _origin;
	const Vector2D _deltaVector = _direction * _delta;
	const Vector2D _outVector2D = _origin + _deltaVector;

	return _outVector2D;
}

Vector2D Vector2D::GetPivotAppliedPosition(Vector2D _position, Vector2D _pivot, int _width, int _height)
{
	const double _pivotX = Helper::Clamp(_pivot.x, 0., 1.);
	const double _pivotY = Helper::Clamp(_pivot.y, 0., 1.);

	const double _startXWorldPosition = _position.x - _width * _pivotX;
	const double _startYWorldPosition = _position.y - _height * _pivotY;

	const Vector2D _outPosition = {_startXWorldPosition, _startYWorldPosition};
	return _outPosition;
}

void Vector2D::GetMinAndMax(Vector2D& _outMin, Vector2D& _outMax, Vector2D _vector1, Vector2D _vector2)
{
	double _minX, _maxX;
	double _minY, _maxY;

	if (_vector1.x <= _vector2.x)
	{
		_maxX = _vector2.x;
		_minX = _vector1.x;
	}
	else
	{
		_maxX = _vector1.x;
		_minX = _vector2.x;
	}

	if (_vector1.y <= _vector2.y)
	{
		_maxY = _vector2.y;
		_minY = _vector1.y;
	}
	else
	{
		_maxY = _vector1.y;
		_minY = _vector2.y;
	}

	_outMin = {_minX, _minY};
	_outMax = {_maxX, _maxY};
}
