#include "AI.hpp"

#include <stdexcept>

const std::string AI::myTeam = "myTeam";
const std::string AI::opponentTeam = "opponentTeam";

std::vector<AI*> AI::instances;

void AI::registerInstance(AI* instance)
{
	instances.push_back(instance);
}

void AI::removeInstance(AI* instance)
{
	for (auto it = std::begin(AI::instances); it != std::end(AI::instances); ++it)
	{
		if ((*it)->l == instance->l)
		{
			instances.erase(it);
			break;
		}
	}
}

AI* AI::getInstance(lua_State* l)
{
	AI* result = nullptr;

	for (AI* instance : instances)
	{
		if (instance->l == l)
		{
			result = instance;
			break;
		}
	}

	return result;
} 

int AI::shootStatic(lua_State* l)
{
	return getInstance(l)->shoot();
}

int AI::snatchStatic(lua_State* l)
{
	return getInstance(l)->snatch();
}

int AI::setSpeedStatic(lua_State* l)
{
	return getInstance(l)->setSpeed();
}

int AI::printStatic(lua_State* l)
{
	return getInstance(l)->print();
}