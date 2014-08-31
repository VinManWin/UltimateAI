#pragma once

#include <SFML/System/Vector2.hpp>
struct lua_State;
#include "scrTeam.hpp"

namespace scr
{
	struct Disc
	{
		bool caught;
		Team::Owner owner;
		unsigned int player;
		sf::Vector2f position;
		sf::Vector2f speed;
		float radius;

		static void registerMetatable(lua_State* l);
		static void create(lua_State* l);
		static void createOnStack(lua_State* l);
		static void set(lua_State* l, const Disc& d);
		static void setOnStack(lua_State* l, const Disc& d);
		static void push(lua_State* l);
		static bool check(lua_State* l, int index);
	};
}