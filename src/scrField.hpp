#pragma once

struct lua_State;

namespace scr
{
	struct Field
	{
		float width;
		float height;
		float zoneWidth;

		float boundLvl;
		float boundExtent;
		float zoneLvl;

		static void registerMetatable(lua_State* l);
		static void create(lua_State* l);
		static void createOnStack(lua_State* l);
		static void set(lua_State* l, const Field& f);
		static void setOnStack(lua_State* l, const Field& f);
		static void push(lua_State* l);
		static bool check(lua_State* l, int index);
	};
}