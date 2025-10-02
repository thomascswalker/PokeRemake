#pragma once

template <typename T>
class ISingleton
{
protected:
	ISingleton() = default;
	~ISingleton() = default;

public:
	ISingleton(const ISingleton&) = delete;
	ISingleton& operator=(const ISingleton&) = delete;

	static T* Instance()
	{
		static T sInstance;
		return &sInstance;
	}
};