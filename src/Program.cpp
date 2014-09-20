#include "Program.hpp"

#include <iostream>
#include "make_unique.hpp"

Program::Program(std::vector<std::string> args) :
	wnd(sf::VideoMode(800, 600, 32), "Ultimate AI", 7, sf::ContextSettings(0,0,8,2,0)),
	keepRunning(true)
{
	std::string fileA, fileB;

	fileA = "res/scripts/default.lua";
	fileB = fileA;

	if (args.size() >= 3)
	{
		if (std::ifstream(args[1]).good())
			fileA = args[1];
		else
			std::cerr << "Could not open '" << args[1] << "'; continue using '" << fileA << "' instead." << std::endl;

		if (std::ifstream(args[2]).good())
			fileB = args[2];
		else
			std::cerr << "Could not open '" << args[2] << "'; continue using '" << fileB << "' instead." << std::endl;
	}

	match = unique<Match>(wnd, fileA, fileB);
}

void Program::run()
{
	sf::Clock clock;
	sf::Time frameTime = sf::milliseconds(1000 / 60);

	while (keepRunning && wnd.isOpen())
	{
		sf::Time t = clock.restart();

		doOneFrame(t);

		if (t < frameTime)
			sf::sleep(frameTime - t);
	}
}
void Program::doOneFrame(sf::Time t)
{
	sf::Event e;
	while (wnd.pollEvent(e))
	{
		handleEvent(e);
	}

	update(t);
	draw();
}

void Program::handleEvent(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::Closed:
		keepRunning = false;
		break;
	}

	match->handleEvent(e);
}

void Program::update(sf::Time t)
{
	match->update(t);
}

void Program::draw()
{
	wnd.clear();
	match->draw();

	wnd.display();
}