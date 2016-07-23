#pragma once


namespace realtrick
{
	template<typename T>
	class Singleton
	{
	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton &) = delete;
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;
	public:
		static T& getInstance()
		{
			static T instance;
			return instance;
		}
	};
}