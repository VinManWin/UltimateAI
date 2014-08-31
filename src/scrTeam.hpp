#pragma once

#include <SFML/System/Vector2.hpp>
struct lua_State;

namespace scr
{
	struct Team
	{
		enum Owner
		{
			Mine,
			Opponent
		};

		Owner owner;
		unsigned int score;

		static void registerMetatable(lua_State* l);
		static void create(lua_State* l, Team::Owner owner);
		static void createOnStack(lua_State* l);
		static void set(lua_State* l, const Team& t);
		static void setOnStack(lua_State* l, const Team& t);
		static void push(lua_State* l, Owner owner);
		static bool check(lua_State* l, int index);
	};
}