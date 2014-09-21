#include "Program.hpp"

#include <iostream>
#include "make_unique.hpp"

void showUsage(std::string arg0)
{
	std::cerr << "Usage: " << arg0 << " [scriptfileA [scriptfileB]]" << std::endl;
}

void checkFile(std::string& name, const std::string arg)
{
	if (std::ifstream(arg).good())
		name = arg;
	else
		std::cerr << "Could not open '" << arg << "'; continue using '" << name << "' instead." << std::endl;
}

Program::Program(std::vector<std::string> args) :
	keepRunning(true)
{
	if (args[1] == "-h" || args[1] == "--help")
	{
		showUsage(args[0]);
		keepRunning = false;
	}
	else
	{
		wnd = unique<sf::RenderWindow>(sf::VideoMode(800, 600, 32), "Ultimate AI", 7, sf::ContextSettings(0, 0, 8, 2, 0));
		std::string fileA, fileB;

		fileA = "res/scripts/default.lua";
		fileB = fileA;

		if (args.size() >= 2)
		{
			checkFile(fileA, args[1]);

			if (args.size() >= 3)
				checkFile(fileB, args[2]);
		}

		std::cout << "Playing: " << fileA << " vs. " << fileB << std::endl;

		match = unique<Match>(*wnd, fileA, fileB);
	}
}

void Program::run()
{
	sf::Clock clock;
	sf::Time frameTime = sf::milliseconds(1000 / 60);

	while (keepRunning && wnd->isOpen())
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
	while (wnd->pollEvent(e))
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
	wnd->clear();
	match->draw();

	wnd->display();
}