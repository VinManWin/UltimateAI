#pragma once

#include <stdexcept>
#include <string>

class LuaError :
	public std::runtime_error
{
public:
	explicit LuaError(const std::string& what_arg);
	explicit LuaError(const char* what_arg);
};

