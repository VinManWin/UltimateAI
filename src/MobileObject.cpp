#include "MobileObject.hpp"

#include <SFML/System/Time.hpp>

sf::Vector2f MobileObject::getDisplacement(sf::Time t)
{
	return velocity * t.asSeconds();
}

void MobileObject::setVelocity(sf::Vector2f speed)
{
	this->velocity = speed;
}

sf::Vector2f MobileObject::getVelocity() const
{
	return velocity;
}