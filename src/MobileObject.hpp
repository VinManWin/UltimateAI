#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
namespace sf { class Time; }

class MobileObject : public sf::Transformable, public sf::Drawable
{
	sf::Vector2f velocity;

public:
	sf::Vector2f getDisplacement(sf::Time t);
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getVelocity() const;
};

