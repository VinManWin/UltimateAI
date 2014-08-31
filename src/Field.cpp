#include "Field.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

const unsigned int Field::width = 100;
const unsigned int Field::endzoneWidth = 18;
const unsigned int Field::brickPointToEndzone = 20;
const unsigned int Field::height = 37;

Field::Field()
{
	const float lineWidth = .2;

	sf::Vector2f offset(-(width / 2.f), -(height / 2.f));

	field.setPosition(offset.x, offset.y);
	field.setSize(sf::Vector2f(width, height));
	field.setFillColor(sf::Color::Green);

	outlines.setPosition(lineWidth + offset.x, lineWidth + offset.y);
	outlines.setSize(sf::Vector2f(width - 2 * lineWidth, height - 2 * lineWidth));
	outlines.setFillColor(sf::Color::Transparent);
	outlines.setOutlineColor(sf::Color::White);
	outlines.setOutlineThickness(lineWidth);
	
	endzoneLines.setPosition(endzoneWidth + lineWidth + offset.x, lineWidth + offset.y);
	endzoneLines.setSize(sf::Vector2f(width - (2 * endzoneWidth) - 2 * lineWidth, height - 2 * lineWidth));
	endzoneLines.setFillColor(sf::Color::Transparent);
	endzoneLines.setOutlineColor(sf::Color::White);
	endzoneLines.setOutlineThickness(lineWidth);

	leftBrickPoint.setPosition(endzoneWidth + brickPointToEndzone + offset.x, height / 2 + offset.y);
	leftBrickPoint.setRadius(lineWidth);

	rightBrickPoint.setPosition(width - (endzoneWidth + brickPointToEndzone) + offset.x, height / 2 + offset.y);
	rightBrickPoint.setRadius(lineWidth);
}


Field::~Field()
{
}

sf::Vector2f Field::getSize() const
{
	return sf::Vector2f(width, height);
}

sf::FloatRect Field::getVisualBounds() const
{
	return sf::FloatRect(getPosition() - getSize() * 0.5f, getSize());
}

sf::FloatRect Field::getPlayBounds() const
{
	return sf::FloatRect(getPosition() - getSize() * .5f, getSize());
}


void Field::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(field, states);
	target.draw(endzoneLines, states);
	target.draw(outlines, states);
	target.draw(leftBrickPoint, states);
	target.draw(rightBrickPoint, states);
}