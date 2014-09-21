#pragma once

#include <SFML/Graphics.hpp>
#include "Match.hpp"

class Program
{
	bool keepRunning;
	std::unique_ptr<sf::RenderWindow> wnd;
	std::unique_ptr<Match> match;

	void doOneFrame(sf::Time t);
	void handleEvent(sf::Event e);
	void update(sf::Time t);
	void draw();

public:
	Program(std::vector<std::string>);

	void run();
};

