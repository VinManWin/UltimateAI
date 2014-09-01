#include "scrPlayer.hpp"

#include <string>
#include <lua.hpp>
#include "scrUtility.hpp"
#include "scrVector2.hpp"

namespace scr
{
	const std::string metatableName = "metaPlayer";

	void Player::registerMetatable(lua_State* l)
	{
		if (luaL_newmetatable(l, metatableName.c_str()) == 1)
		{
		}
	}

	void Player::create(lua_State* l, Team::Owner owner, unsigned int number)
	{
		Team::push(l, owner);
		lua_pushnumber(l, number);
		Player::createOnStack(l);
		lua_settable(l, -3);
	}

	void Player::createOnStack(lua_State* l)
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

	void Player::set(lua_State* l, const Player& player)
	{
		push(l, player.owner, player.number);
		setOnStack(l, player);
		lua_pop(l, 1);
	}

	void Player::setOnStack(lua_State* l, const Player& player)
	{
		lua_pushstring(l, "team");
		Team::push(l, player.owner);
		lua_settable(l, -3);

		lua_pushstring(l, "number");
		lua_pushnumber(l, player.number);
		lua_settable(l, -3);

		lua_pushstring(l, "position");
		lua_gettable(l, -2);
		scr::vector2::setOnStack(l, player.position);
		lua_pop(l, 1);

		lua_pushstring(l, "velocity");
		lua_gettable(l, -2);
		scr::vector2::setOnStack(l, player.velocity);
		lua_pop(l, 1);
	}

	void Player::push(lua_State* l, Team::Owner owner, unsigned int number)
	{
		scr::Team::push(l, owner);

		lua_pushstring(l, "players");
		lua_gettable(l, -2);

		lua_pushinteger(l, number);
		lua_gettable(l, -2);

		lua_replace(l, -3);
		lua_pop(l, 1);
	}

	bool Player::check(lua_State* l, int index)
	{
		return lua_istable(l, index) && scr::hasMetatable(l, index, metatableName);
	}
}