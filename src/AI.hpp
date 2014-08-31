#pragma once

#include <string>
#include <vector>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <lua.hpp>
#include "scrTypes.hpp"
#include "Logfile.hpp"
enum class PlayDirection;

class AI
{
public:
	typedef std::function<bool(unsigned int, sf::Vector2f)> ShootFunction;
	typedef std::function<bool(unsigned int)> SnatchFunction;
	typedef std::function<bool(unsigned int, sf::Vector2f)> SetSpeedFunction;

private:
	lua_State* l;
	unsigned int teamSize;
	Logfile logfile;

	ShootFunction shootCallback;
	SnatchFunction snatchCallback;
	SetSpeedFunction setSpeedCallback;

	static const std::string myTeam;
	static const std::string opponentTeam;

	static std::vector<AI*> instances;
	static void registerInstance(AI* instance);
	static void removeInstance(AI* instance);
	static AI* getInstance(lua_State* l);

	static int shootStatic(lua_State* l);
	static int snatchStatic(lua_State* l);
	static int setSpeedStatic(lua_State* l);
	static int printStatic(lua_State* l);

	void logError(std::string s);
	void tryCall(std::string funcName, unsigned int argc, unsigned int retc);

	int shoot();
	int snatch();
	int setSpeed();
	int print();

public:
	AI(std::string file, size_t teamSize);
	~AI();

	void setShootCallback(ShootFunction func);
	void setSnatchCallback(SnatchFunction func);
	void setSetSpeedCallback(SetSpeedFunction func);

	void set(const scr::Globals& globals);
	void set(const scr::Field& field);
	void set(const scr::Disc& disc);
	void set(const scr::Player& player);
	void set(const scr::Team& team);

	void callGlobalScope();
	std::vector<sf::Vector2f> getFormation(bool startingTeam);
	void start();
	void update(float seconds);
	void onDiscWithinReach(unsigned int playerNumber);
	void onOpponentShot();
	void onOpponentSnatch(bool score);

	void log(std::string msg, Logfile::EntryType et);
};

