#pragma once
#include <memory>

namespace Engine
{
	struct Vector2D
	{
	public:
		Vector2D() = default;

		Vector2D(double _x, double _y);

		Vector2D operator+(Vector2D _other) const;

		Vector2D operator-(Vector2D _other) const;

		Vector2D operator*(double _scalar) const;

		Vector2D operator/(double _scalar) const;

		bool operator==(const Vector2D& _other) const;

		bool operator!=(const Vector2D& _other) const;

		Vector2D Normalize() const;

		double Magnitude() const;

		double DistanceTo(Vector2D& _other) const;

	public:
		double x;
		double y;

	public:
		static const Vector2D Left;
		static const Vector2D Right;
		static const Vector2D Up;
		static const Vector2D Down;
		static const Vector2D RightUp;
		static const Vector2D RightDown;
		static const Vector2D LeftUp;
		static const Vector2D LeftDown;
		static const Vector2D Zero;
		static const Vector2D Center;
		static const Vector2D One;

		static Vector2D Lerp(Vector2D _origin, Vector2D _destination, double _delta);

		static Vector2D GetPivotAppliedPosition(Vector2D _position, Vector2D _pivot, int _width, int _height);

		static void GetMinAndMax(Vector2D& _outMin, Vector2D& _outMax, Vector2D _vector1, Vector2D _vector2);
	};
}

namespace std
{
	template <>
	struct hash<Engine::Vector2D>
	{
		std::size_t operator()(const Engine::Vector2D& _value) const noexcept
		{
			return ((std::hash<double>()(_value.x)
				^ (std::hash<double>()(_value.y) << 1)) >> 1);
		}
	};
}
