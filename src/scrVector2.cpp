#include "scrVector2.hpp"

#include <string>
#include <lua.hpp>
#include "scrUtility.hpp"

namespace scr
{
	namespace vector2
	{
		const std::string metatableName = "metaVector2";

		void registerMetatable(lua_State* l)
		{
			if (luaL_newmetatable(l, metatableName.c_str()) == 1)
			{
				lua_pushstring(l, "_unm");
				lua_pushcfunction(l, exp::neg);
				lua_settable(l, -3);

				lua_pushstring(l, "__add");
				lua_pushcfunction(l, exp::add);
				lua_settable(l, -3);

				lua_pushstring(l, "__sub");
				lua_pushcfunction(l, exp::sub);
				lua_settable(l, -3);

				lua_pushstring(l, "__mul");
				lua_pushcfunction(l, exp::mul);
				lua_settable(l, -3);

				lua_pushstring(l, "__div");
				lua_pushcfunction(l, exp::div);
				lua_settable(l, -3);

				// __index
				lua_pushstring(l, "__index");
				lua_newtable(l);

				lua_pushstring(l, "length");
				lua_pushcfunction(l, exp::len);

				lua_settable(l, -3);

				lua_pushstring(l, "norm");
				lua_pushcfunction(l, exp::norm);
				lua_settable(l, -3);

				//lua_pushstring(l, "dotP");
				//lua_pushcfunction(l, vector2::dotP);
				//lua_settable(l, -3);


				lua_settable(l, -3);

				lua_pushstring(l, "__tostring");
				lua_pushcfunction(l, vector2::exp::toString);
				lua_settable(l, -3);
			}
		}

		void createOnStack(lua_State* l)
		{
			lua_newtable(l);
			luaL_getmetatable(l, metatableName.c_str());
			lua_setmetatable(l, -2);
		}

		void setOnStack(lua_State* l, float x, float y)
		{
			lua_pushstring(l, "x");
			lua_pushnumber(l, x);
			lua_settable(l, -3);

			lua_pushstring(l, "y");
			lua_pushnumber(l, y);
			lua_settable(l, -3);
		}

		void setOnStack(lua_State* l, sf::Vector2f v)
		{
			setOnStack(l, v.x, v.y);
		}

		sf::Vector2f getFromStack(lua_State* l, int index)
		{
			sf::Vector2f v;

			lua_pushvalue(l, index);

			lua_pushstring(l, "x");
			lua_gettable(l, -2);
			v.x = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_pushstring(l, "y");
			lua_gettable(l, -2);
			v.y = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_pop(l, 1);

			return v;
		}

		bool check(lua_State* l, int index)
		{
			return lua_istable(l, index) && scr::hasMetatable(l, index, metatableName.c_str());
		}

	}
}