#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <stdexcept>

namespace Engine
{
	template <typename ... Args>
	std::string string_format(const std::string& format, Args ... args)
	{
		int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
		auto size = static_cast<size_t>(size_s);
		auto buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	}

	template <class TClass>
	class ObjectBase
	{
	public:
		ObjectBase();

		~ObjectBase();

	private:
		std::string m_Name;

		int m_Id;

	public:
		inline std::string GetName() const { return m_Name; }

		inline void SetName(std::string _name) { m_Name = _name; }

		inline int GetId() const { return m_Id; }

	private:
		static int UniqueIndex;
	};

	template <class TClass>
	int ObjectBase<TClass>::UniqueIndex = 0;

	template <class TClass>
	ObjectBase<TClass>::ObjectBase()
	{
		++UniqueIndex;

		m_Id = UniqueIndex;

		std::string _name = string_format("%s_%d", typeid(TClass).name(), m_Id);
		m_Name = _name;
	}

	template <class TClass>
	ObjectBase<TClass>::~ObjectBase()
	{
	}
}
