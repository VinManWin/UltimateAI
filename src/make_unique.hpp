#pragma once
#include <memory>

template<typename T, typename ...Args>
std::unique_ptr<T> unique(Args&& ...args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}