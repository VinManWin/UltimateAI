#include "scrVector2.hpp"

#include <string>
#include <sstream>
#include <lua.hpp>
#include "scrUtility.hpp"
#include "utility.hpp"

namespace scr
{
	namespace vector2
	{
		namespace exp
		{
			void checkArgVec(lua_State* l, const std::string& fncName)
			{
				if (!vector2::check(l, 1))
					luaL_error(l, ("error calling function 'ult.Vector2." + fncName + "': argument error, arguments are ('Vector2').").c_str());
			}

			void checkArgVecVec(lua_State* l, const std::string& fncName)
			{
				if (!scr::vector2::check(l, 1) || !scr::vector2::check(l, 2))
					luaL_error(l, ("error calling function 'ult.Vector2." + fncName + "': argument error, arguments are ('Vector2', 'Vector2').").c_str());
			}

			int create(lua_State* l)
			{
				scr::vector2::createOnStack(l);

				if (lua_isnumber(l, 1) && lua_isnumber(l, 2))
				{
					vector2::setOnStack(l, lua_tonumber(l, 1), lua_tonumber(l, 2));
				}
				else
				{
					vector2::setOnStack(l, lua_tonumber(l, 1), lua_tonumber(l, 2));
				}

				return 1;
			}

			int copy(lua_State* l)
			{
				checkArgVec(l, "copy");

				sf::Vector2f v = vector2::getFromStack(l, 1);
				lua_pop(l, 1);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, v);

				return 1;
			}

			int neg(lua_State* l)
			{
				checkArgVec(l, "neg");

				sf::Vector2f v = scr::vector2::getFromStack(l, 1);
				lua_pop(l, 1);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, -v);

				return 1;
			}

			int add(lua_State* l)
			{
				checkArgVecVec(l, "add");

				sf::Vector2f v1 = scr::vector2::getFromStack(l, 1);
				sf::Vector2f v2 = scr::vector2::getFromStack(l, 2);
				lua_pop(l, 2);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, v1 + v2);

				return 1;
			}

			int sub(lua_State* l)
			{
				checkArgVecVec(l, "sub");

				sf::Vector2f v1 = scr::vector2::getFromStack(l, 1);
				sf::Vector2f v2 = scr::vector2::getFromStack(l, 2);
				lua_pop(l, 2);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, v1 - v2);

				return 1;
			}

			int mul(lua_State* l)
			{
				checkArgVec(l, "mul");

				sf::Vector2f v = scr::vector2::getFromStack(l, 1);
				float f = lua_tonumber(l, 2);
				lua_pop(l, 2);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, v * f);

				return 1;
			}

			int div(lua_State* l)
			{
				checkArgVec(l, "div");

				sf::Vector2f v = scr::vector2::getFromStack(l, 1);
				float f = lua_tonumber(l, 2);
				lua_pop(l, 2);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, v / f);

				return 1;
			}

			int len(lua_State* l)
			{
				checkArgVec(l, "len");

				sf::Vector2f v = scr::vector2::getFromStack(l, 1);
				lua_pop(l, 1);

				scr::vector2::createOnStack(l);
				lua_pushnumber(l, ::len(v));

				return 1;
			}

			int norm(lua_State* l)
			{
				checkArgVec(l, "norm");

				sf::Vector2f v = scr::vector2::getFromStack(l, 1);
				lua_pop(l, 1);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, ::norm(v));

				return 1;
			}

			int dotP(lua_State* l)
			{
				checkArgVecVec(l, "dotP");

				sf::Vector2f v1 = scr::vector2::getFromStack(l, 1);
				sf::Vector2f v2 = scr::vector2::getFromStack(l, 2);
				lua_pop(l, 2);

				scr::vector2::createOnStack(l);
				scr::vector2::setOnStack(l, v1.x * v1.y, v1.y * v2.y);

				return 1;
			}

			int toString(lua_State* l)
			{
				checkArgVec(l, "toString");
				sf::Vector2f v = scr::vector2::getFromStack(l, 1);
				lua_pop(l, 1);
				std::stringstream s;
				s << "(" << v.x << " | " << v.y << ")";

				lua_pushstring(l, s.str().c_str());
				return 1;
			}
		}
	}
}