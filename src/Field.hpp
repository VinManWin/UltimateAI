#pragma once

#include <type_traits>
#include <vector>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "PlayDirection.hpp"

class Field : public sf::Drawable, public sf::Transformable
{
public:
	static const unsigned int width;
	static const unsigned int endzoneWidth;
	static const unsigned int brickPointToEndzone;
	static const unsigned int height;

	enum class Location
	{
		MidZone,
		LeftZone,
		RightZone,
		OutOfBounds
	};

private:
	sf::RectangleShape field;
	sf::RectangleShape outlines;
	sf::RectangleShape endzoneLines;
	sf::CircleShape leftBrickPoint;
	sf::CircleShape rightBrickPoint;

public:
	Field();
	virtual ~Field();

	sf::Vector2f getSize() const;
	sf::FloatRect getVisualBounds() const;
	sf::FloatRect getPlayBounds() const;

	template <typename T>
	sf::Vector2<T> convertToRel(sf::Vector2<T> v, PlayDirection dir) const;

	template <typename T>
	Location locate(sf::Vector2<T> pos) const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


template<typename T>
sf::Vector2<T> Field::convertToRel(sf::Vector2<T> v, PlayDirection dir) const
{
	return dir == PlayDirection::toRight ? v : -v;
}

template <typename T>
Field::Location Field::locate(sf::Vector2<T> pos) const
{
	Location loc;

	if (abs(pos.x) > width / 2 || abs(pos.y) > height / 2)
	{
		loc = Location::OutOfBounds;
	}
	else if (pos.x <= -(int)(width / 2 - endzoneWidth))
	{
		loc = Location::LeftZone;
	}
	else if (pos.x >= (width / 2 - endzoneWidth))
	{
		loc = Location::RightZone;
	}
	else
	{
		loc = Location::MidZone;
	}
	return loc;
}