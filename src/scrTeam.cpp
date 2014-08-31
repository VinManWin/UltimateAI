#include "scrTeam.hpp"

#include <string>
#include <cassert>
#include <lua.hpp>
#include "scrUtility.hpp"
#include "scrVector2.hpp"
#include "scrPlayer.hpp"

namespace scr
{
	const std::string metatableName = "metaTeam";
	const std::string myTeam = "myTeam";
	const std::string opponentTeam = "opponentTeam";

	std::string getGlobalName(Team::Owner owner)
	{
		switch (owner)
		{
		case scr::Team::Mine:
			return myTeam;
			break;

		case scr::Team::Opponent:
			return opponentTeam;
			break;

		default:
			assert(false);
			return "";
			break;
		}
	}

	void Team::registerMetatable(lua_State* l)
	{
		if (luaL_newmetatable(l, metatableName.c_str()) == 1)
		{
		}
	}

	void Team::create(lua_State* l, Team::Owner owner)
	{
		createOnStack(l);
		lua_setglobal(l, getGlobalName(owner).c_str());
	}

	void Team::createOnStack(lua_State* l)
	{
		lua_newtable(l);

		lua_pushstring(l, "players");
		lua_newtable(l);

		for (size_t i = 1; i <= 7; ++i)
		{
			lua_pushinteger(l, i);
			scr::Player::createOnStack(l);
			lua_settable(l, -3);
		}

		lua_settable(l, -3);

		luaL_getmetatable(l, metatableName.c_str());
		lua_setmetatable(l, -2);
	}

	void Team::set(lua_State* l, const Team& team)
	{
		push(l, team.owner);
		setOnStack(l, team);
		lua_pop(l, 1);
	}

	void Team::setOnStack(lua_State* l, const Team& team)
	{
		lua_pushstring(l, "score");
		lua_pushnumber(l, team.score);
		lua_settable(l, -3);
	}

	void Team::push(lua_State* l, Owner owner)
	{
		lua_getglobal(l, getGlobalName(owner).c_str());
	}

	bool Team::check(lua_State* l, int index)
	{
		return lua_istable(l, index) && scr::hasMetatable(l, index, metatableName);
	}
}