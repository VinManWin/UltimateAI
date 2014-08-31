#include "Timer.hpp"

Timer::Timer() :
running(false)
{
}

Timer::Timer(std::function<void()> callback, sf::Time t) :
endTime(t),
lastCaptureTime(sf::milliseconds(0)),
callback(callback),
running(t != sf::milliseconds(0))
{
}

void Timer::setCallback(std::function<void()> callback)
{
	this->callback = callback;
}

void Timer::set(sf::Time t)
{
	endTime = t;
	lastCaptureTime = sf::milliseconds(0);
}

void Timer::start()
{
	running = true;
}

void Timer::pause()
{
	running = false;
}

void Timer::capture(sf::Time elapsedTime)
{
	if (running)
	{
		lastCaptureTime += elapsedTime;

		if (lastCaptureTime >= endTime)
		{
			running = false;
			callback();
		}
	}
}