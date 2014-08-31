#pragma once
#include <fstream>
#include <SFML/System/Clock.hpp>

class Logfile
{
public:
	enum EntryType
	{
		Error,
		Warning,
		Info
	};

private:
	std::ofstream stream;
	sf::Clock clock;

	std::string getPrefix(EntryType et) const;
	std::string getTime() const;

public:
	Logfile(std::string filename);
	void write(std::string msg, EntryType et);
};