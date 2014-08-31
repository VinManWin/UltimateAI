#pragma once

struct lua_State;

namespace scr
{
	struct Globals
	{
		float playerMaxSpeed;
		float playerRadius;

		float discMaxSpeed, discMinSpeed;
		float discRadius;

		static void set(lua_State* l, const Globals& g);
	};
}