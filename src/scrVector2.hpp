#pragma once

#include <SFML/System/Vector2.hpp>
struct lua_State;

namespace scr
{
	namespace vector2
	{
		void registerMetatable(lua_State* l);
		void createOnStack(lua_State* l);
		void setOnStack(lua_State* l, float x, float y);
		void setOnStack(lua_State* l, sf::Vector2f v);
		sf::Vector2f getFromStack(lua_State* l, int index);
		bool check(lua_State* l, int index);

		namespace exp
		{
			int create(lua_State* l);
			int copy(lua_State* l);
			int neg(lua_State* l);
			int add(lua_State* l);
			int sub(lua_State* l);
			int mul(lua_State* l);
			int div(lua_State* l);
			int len(lua_State* l);
			int norm(lua_State* l);
			int dotP(lua_State* l);
			int toString(lua_State* l);
		}
	}
}