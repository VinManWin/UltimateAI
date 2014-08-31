#include "scrGlobals.hpp"

#include <lua.hpp>

void scr::Globals::set(lua_State* l, const Globals& g)
{
	lua_newtable(l);
	lua_pushstring(l, "maxSpeed");
	lua_pushnumber(l, g.playerMaxSpeed);
	lua_settable(l, -3);
	lua_pushstring(l, "radius");
	lua_pushnumber(l, g.playerRadius);
	lua_settable(l, -3);
	lua_setglobal(l, "Player");

	lua_newtable(l);
	lua_pushstring(l, "minSpeed");
	lua_pushnumber(l, g.discMinSpeed);
	lua_settable(l, -3);
	lua_pushstring(l, "maxSpeed");
	lua_pushnumber(l, g.discMaxSpeed);
	lua_settable(l, -3);
	lua_pushstring(l, "radius");
	lua_pushnumber(l, g.discRadius);
	lua_settable(l, -3);
	lua_setglobal(l, "Disc");
}