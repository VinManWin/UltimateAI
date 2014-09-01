#include "scrDisc.hpp"

#include <string>
#include <lua.hpp>
#include "scrUtility.hpp"
#include "scrVector2.hpp"
#include "scrPlayer.hpp"

namespace scr
{
	const std::string metatableName = "metaDisc";

	void Disc::registerMetatable(lua_State* l)
	{
		if (luaL_newmetatable(l, metatableName.c_str()) == 1)
		{
		}
	}

	void Disc::create(lua_State* l)
	{
		createOnStack(l);
		lua_setglobal(l, "disc");
	}

	void Disc::createOnStack(lua_State* l)
	{
		lua_newtable(l);

		lua_pushstring(l, "position");
		scr::vector2::createOnStack(l);
		lua_settable(l, -3);

		lua_pushstring(l, "velocity");
		scr::vector2::createOnStack(l);
		lua_settable(l, -3);

		luaL_getmetatable(l, metatableName.c_str());
		lua_setmetatable(l, -2);
	}

	void Disc::set(lua_State* l, const Disc& disc)
	{
		push(l);
		setOnStack(l, disc);
		lua_pop(l, 1);
	}

	void Disc::setOnStack(lua_State* l, const Disc& disc)
	{
		if (disc.caught)
		{
			lua_pushstring(l, "possessingPlayer");
			Player::push(l, disc.owner, disc.player);
			lua_settable(l, -3);
		}
		else
		{
			lua_pushstring(l, "possessingPlayer");
			lua_pushnil(l);
			lua_settable(l, -3);
		}

		lua_pushstring(l, "position");
		lua_gettable(l, -2);
		scr::vector2::setOnStack(l, disc.position);
		lua_pop(l, 1);

		lua_pushstring(l, "velocity");
		lua_gettable(l, -2);
		scr::vector2::setOnStack(l, disc.velocity);
		lua_pop(l, 1);

		lua_pushstring(l, "radius");
		lua_pushnumber(l, disc.radius);
		lua_settable(l, -3);
	}

	void Disc::push(lua_State* l)
	{
		lua_getglobal(l, "disc");
	}

bool Disc::check(lua_State* l, int index)
{
	return lua_istable(l, index) && scr::hasMetatable(l, index, metatableName);
}
}