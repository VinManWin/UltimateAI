#include "scrField.hpp"

#include <string>
#include <lua.hpp>
#include "scrUtility.hpp"

namespace scr
{
	const std::string metatableName = "metaField";

	void Field::registerMetatable(lua_State* l)
	{
		if (luaL_newmetatable(l, metatableName.c_str()) == 1)
		{
		}
	}

	void Field::create(lua_State* l)
	{
		createOnStack(l);
		lua_setglobal(l, "field");
	}

	void Field::createOnStack(lua_State* l)
	{
		lua_newtable(l);

		luaL_getmetatable(l, metatableName.c_str());
		lua_setmetatable(l, -2);
	}

	void Field::set(lua_State* l, const Field& field)
	{
		push(l);
		setOnStack(l, field);
		lua_pop(l, 1);
	}

	void Field::setOnStack(lua_State* l, const Field& field)
	{

		lua_pushstring(l, "width");
		lua_pushnumber(l, field.width);
		lua_settable(l, -3);

		lua_pushstring(l, "height");
		lua_pushnumber(l, field.height);
		lua_settable(l, -3);

		lua_pushstring(l, "zoneWidth");
		lua_pushnumber(l, field.zoneWidth);
		lua_settable(l, -3);

		lua_pushstring(l, "boundLvl");
		lua_pushnumber(l, field.boundLvl);
		lua_settable(l, -3);

		lua_pushstring(l, "boundExtent");
		lua_pushnumber(l, field.boundExtent);
		lua_settable(l, -3);

		lua_pushstring(l, "zoneLvl");
		lua_pushnumber(l, field.zoneLvl);
		lua_settable(l, -3);
	}

	void Field::push(lua_State* l)
	{
		lua_getglobal(l, "field");
	}

	bool Field::check(lua_State* l, int index)
	{
		return lua_istable(l, index) && scr::hasMetatable(l, index, metatableName);
	}
}