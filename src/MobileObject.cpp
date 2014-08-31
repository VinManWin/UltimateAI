#include "MobileObject.hpp"

#include <SFML/System/Time.hpp>

sf::Vector2f MobileObject::getDisplacement(sf::Time t)
{
	return speed * t.asSeconds();
}

void MobileObject::setSpeed(sf::Vector2f speed)
{
	this->speed = speed;
}

sf::Vector2f MobileObject::getSpeed() const
{
	return speed;
}