#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
namespace sf { class Time; }

class MobileObject : public sf::Transformable, public sf::Drawable
{
	sf::Vector2f speed;

public:
	sf::Vector2f getDisplacement(sf::Time t);
	void setSpeed(sf::Vector2f speed);
	sf::Vector2f getSpeed() const;
};

