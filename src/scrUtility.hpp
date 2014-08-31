#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>
struct lua_State;

namespace scr
{
	std::string valueToString(lua_State* l, int index);
	void dumpStack(lua_State* l);
	void openLib(lua_State* l);
	bool hasMetatable(lua_State* l, int index, std::string metatable);
}