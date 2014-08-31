#include "scrUtility.hpp"

#include <iostream>
#include <sstream>
#include <SFML/System/Vector2.hpp>
#include <lua.hpp>
#include "scrTypes.hpp"

namespace scr
{
	std::string valueToString(lua_State* l, int index)
	{
		//std::stringstream s;

		//int t = lua_type(l, index);
		//switch (t) {

		//case LUA_TSTRING:  /* strings */
		//	s << "'" << lua_tostring(l, index) << "'";
		//	break;

		//case LUA_TBOOLEAN:  /* booleans */
		//	s << lua_toboolean(l, index) ? "true" : "false";
		//	break;

		//case LUA_TNUMBER:  /* numbers */
		//	s << lua_tonumber(l, index);
		//	break;

		//default:  /* other values */
		//	s << "[" << lua_typename(l, t) << "]";
		//	break;

		//}
		const char* lol = luaL_tolstring(l, index, 0);
		lua_pop(l, 1);
		return lol;
	}

	void dumpStack(lua_State* L) {
		std::cout << ">>";
		int i;
		int top = lua_gettop(L);
		for (i = 1; i <= top; i++) {  /* repeat for each level */
			std::cout << "  ";  /* put a separator */
			std::cout << valueToString(L, i);
		}
		std::cout << std::endl;  /* end the listing */
	}

	void registerMetatables(lua_State* l)
	{
		vector2::registerMetatable(l);
		Field::registerMetatable(l);
		Player::registerMetatable(l);
		Disc::registerMetatable(l);
		Team::registerMetatable(l);
	}

	void openLib(lua_State* l)
	{
		lua_newtable(l);

		lua_pushstring(l, "new");
		lua_pushcfunction(l, scr::vector2::exp::create);
		lua_settable(l, -3);

		lua_setglobal(l, "Vector2");

		registerMetatables(l);
	}

	bool hasMetatable(lua_State* l, int index, std::string metatable)
	{
		lua_getmetatable(l, index);
		luaL_getmetatable(l, metatable.c_str());
		bool result = lua_rawequal(l, -2, -1) > 0;
		lua_pop(l, 2);

		return result;
	}
}