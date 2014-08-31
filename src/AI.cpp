#include "AI.hpp"

#include <sstream>
#include <iostream>
#include "scrUtility.hpp"
#include "LuaError.hpp"

AI::AI(std::string file, size_t teamSize) :
l(luaL_newstate()),
teamSize(teamSize),
logfile(file + ".log")
{
	luaL_openlibs(l);
	registerInstance(this);
	scr::openLib(l);

	lua_pushcfunction(l, printStatic);
	lua_setglobal(l, "print");

	scr::Field::create(l);
	scr::Disc::create(l);
	scr::Team::create(l, scr::Team::Mine);
	scr::Team::create(l, scr::Team::Opponent);

	if (luaL_loadfile(l, file.c_str()))
		logError(lua_tostring(l, -1));
}

AI::~AI()
{
	lua_close(l);
}

void AI::logError(std::string s)
{
	logfile.write(s, Logfile::Error);
	throw LuaError(s);
}

void AI::tryCall(std::string funcName, unsigned int argc, unsigned int retc)
{
	if (lua_pcall(l, argc, retc, 0))
	{
		std::stringstream s;
		s << "error running " << funcName << ": " << lua_tostring(l, -1);
		logError(s.str());
	}
}

void AI::setShootCallback(ShootFunction func)
{
	shootCallback = func;
}

void AI::setSnatchCallback(SnatchFunction func)
{
	snatchCallback = func;
}

void AI::setSetSpeedCallback(SetSpeedFunction func)
{
	setSpeedCallback = func;
}

void AI::set(const scr::Globals& globals)
{
	scr::Globals::set(l, globals);
}

void AI::set(const scr::Field& field)
{
	scr::Field::set(l, field);
}

void AI::set(const scr::Disc& disc)
{
	scr::Disc::set(l, disc); 
}

void AI::set(const scr::Player& player)
{
	scr::Player::push(l, player.owner, player.number);
	scr::Player::setOnStack(l, player);

	lua_pushstring(l, "shoot");
	lua_pushcfunction(l, shootStatic);
	lua_settable(l, -3);

	lua_pushstring(l, "snatch");
	lua_pushcfunction(l, snatchStatic);
	lua_settable(l, -3);

	lua_pushstring(l, "setSpeed");
	lua_pushcfunction(l, setSpeedStatic);
	lua_settable(l, -3);

	lua_pop(l, 1);
}

void AI::set(const scr::Team& team)
{
	scr::Team::set(l, team);
}

void AI::callGlobalScope()
{
	if (lua_pcall(l, 0, 0, 0))
		logError(lua_tostring(l, -1));
}

int AI::shoot()
{
	luaL_checkany(l, 1);
	if (!scr::Player::check(l, 1))
		logError("error calling function 'shoot': first argument is not a 'player'");

	luaL_checkany(l, 2);
	if (!scr::vector2::check(l, 2))
		logError("error calling function 'shoot': second argument is not a '2D vector'");

	sf::Vector2f speed = scr::vector2::getFromStack(l, 2);
	lua_pop(l, 1);

	lua_pushstring(l, "number");
	lua_gettable(l, -2);
	int playerNumber = lua_tointeger(l, -1);
	lua_pop(l, 2);

	bool result = false;
	if (shootCallback)
		result = shootCallback(playerNumber, speed);
	lua_pushboolean(l, result);

	return 0;
}

int AI::snatch()
{
	luaL_checkany(l, 1);
	if (!scr::Player::check(l, 1))
		logError("error calling function 'shoot': first argument is not a 'player'");

	lua_pushstring(l, "number");
	lua_gettable(l, -2);
	int playerNumber = lua_tointeger(l, -1);
	lua_pop(l, 1);

	bool result = false;
	if (snatchCallback)
		result = snatchCallback(playerNumber);
	lua_pushboolean(l, result);

	return 0;
}

int AI::setSpeed()
{
	luaL_checkany(l, 1);
	if (!scr::Player::check(l, 1))
		logError("error calling function 'shoot': first argument is not a 'player'");

	luaL_checkany(l, 2);
	if (!scr::vector2::check(l, 2))
	{
		logError("error calling function 'shoot': second argument is not a '2D vector'");
	}

	sf::Vector2f speed = scr::vector2::getFromStack(l, 2);
	lua_pop(l, 1);

	lua_pushstring(l, "number");
	lua_gettable(l, -2);
	int playerNumber = lua_tointeger(l, -1);
	lua_pop(l, 2);

	bool result = false;
	if (setSpeedCallback)
		result = setSpeedCallback(playerNumber, speed);
	lua_pushboolean(l, result);

	return 0;
}

int AI::print()
{
	for (size_t i = 1; i <= lua_gettop(l); ++i)
		log(scr::valueToString(l, i), Logfile::Info);
	return 0;
}

std::vector<sf::Vector2f> AI::getFormation(bool startingTeam)
{
	std::vector<sf::Vector2f> result;

	std::string funcName = "setup";
	std::string returnErrorText = "function '" + funcName + "' must return a table (array) of tables ({x,y})";

	lua_getglobal(l, funcName.c_str());
	if (lua_isfunction(l, -1))
	{
		lua_pushboolean(l, startingTeam);

		tryCall(funcName, 1, 1);

		if (!lua_istable(l, -1))
			luaL_error(l, returnErrorText.c_str());

		for (size_t i = 1; i <= teamSize; ++i)
		{
			lua_pushinteger(l, i);
			lua_gettable(l, -2);

			if (!scr::vector2::check(l, -1))
				luaL_error(l, returnErrorText.c_str());

			sf::Vector2f v = scr::vector2::getFromStack(l, -1);
			lua_pop(l, 1);

			result.push_back(v);
		}

		lua_pop(l, 1);
	}
	return result;
}

void AI::start()
{
	std::string funcName = "start";
	lua_getglobal(l, funcName.c_str());
	if (lua_isfunction(l, -1))
	{
		tryCall(funcName, 0, 0);
	}
}

void AI::update(float seconds)
{
	std::string funcName = "update";
	lua_getglobal(l, funcName.c_str());
	if (lua_isfunction(l, -1))
	{
		lua_pushnumber(l, seconds);

		tryCall(funcName, 1, 0);
	}
}

void AI::onDiscWithinReach(unsigned int playerNumber)
{
	std::string funcName = "onDiscWithinReach";
	lua_getglobal(l, funcName.c_str());
	if (lua_isfunction(l, -1))
	{
		scr::Player::push(l, scr::Team::Mine, playerNumber);

		tryCall(funcName, 1, 0);
	}
}

void AI::onOpponentShot()
{
	std::string funcName = "onShot";
	lua_getglobal(l, funcName.c_str());
	if (lua_isfunction(l, -1))
	{
		tryCall(funcName, 0, 0);
	}
}

void AI::onOpponentSnatch(bool score)
{
	std::string funcName = "onSnatch";
	lua_getglobal(l, funcName.c_str());
	if (lua_isfunction(l, -1))
	{
		lua_pushboolean(l, score);

		tryCall(funcName, 1, 0);
	}
}

void AI::log(std::string msg, Logfile::EntryType et)
{
	logfile.write(msg, et);
	std::cout << msg << std::endl;
}