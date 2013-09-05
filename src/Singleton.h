#pragma once
template<typename T>
class Singleton
{
public:
	static T & getInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton(void) { };
	~Singleton(void) { };
	Singleton(const Singleton&) { };                 // Prevent copy-construction
	Singleton& operator=(const Singleton&) { };      // Prevent assignment
};

