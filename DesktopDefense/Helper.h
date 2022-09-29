#pragma once

namespace Engine
{
	class Helper
	{
	public:
		template <typename T>
		static T Clamp(T _value, T _min, T _max);
	};

	template <typename T>
	T Helper::Clamp(T _value, T _min, T _max)
	{
		if (_value < _min)
			return _min;

		if (_value > _max)
			return _max;

		return _value;
	}
}
