#pragma once

#include <functional>
#include <vector>
#include <SFML/System/Time.hpp>

class Timer
{
	sf::Time endTime;
	sf::Time lastCaptureTime;
	std::function<void()> callback;
	bool running;

public:
	Timer();
	Timer(std::function<void()> callback, sf::Time t = sf::milliseconds(0));
	void setCallback(std::function<void()> callback);
	void set(sf::Time t);
	void start();
	void pause();
	void capture(sf::Time elapsedTime);
};

