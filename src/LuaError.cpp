#include "LuaError.hpp"


LuaError::LuaError(const std::string& what_arg) : 
std::runtime_error(what_arg)
{
}

LuaError::LuaError(const char* what_arg) :
std::runtime_error(what_arg)
{
}
