#include "Movement.hpp"


Movement::Movement(sf::Vector2f origin, sf::Vector2f displacement) :
origin(origin),
displacement(displacement)
{
}

float greaterThanZeroeOrMax(float f)
{
	return f > 0 ? f : std::numeric_limits<float>::max();
}

float Movement::contactVertically(float barrier)
{
	return greaterThanZeroeOrMax((barrier - origin.x) / displacement.x);
}

float Movement::contactHorizontally(float barrier)
{
	return greaterThanZeroeOrMax((barrier - origin.y) / displacement.y);
}

std::pair<float, Movement::Side> Movement::calcNextContactRatio(sf::FloatRect rect)
{
	float leftContact = contactVertically(rect.left);
	float rightContact = contactVertically(rect.left + rect.width);
	float topContact = contactHorizontally(rect.top);
	float bottomContact = contactHorizontally(rect.top + rect.height);

	Side firstContactedSide = Left;
	float firstContactRatio = leftContact;

	if (rightContact < firstContactRatio)
	{
		firstContactedSide = Right;
		firstContactRatio = rightContact;
	}

	if (topContact < firstContactRatio)
	{
		firstContactedSide = Top;
		firstContactRatio = topContact;
	}

	if (bottomContact < firstContactRatio)
	{
		firstContactedSide = Bottom;
		firstContactRatio = bottomContact;
	}

	return std::make_pair(firstContactRatio, firstContactedSide);
}

std::pair<sf::Vector2f, sf::Vector2f> Movement::deflectVerticallyAt(float ratio)
{
	sf::Vector2f v1 = displacement * ratio;
	sf::Vector2f v2 = displacement * (1 - ratio);
	v2.x = -v2.x;

	return std::make_pair(v1, v2);
}

std::pair<sf::Vector2f, sf::Vector2f> Movement::deflectHorizontallyAt(float ratio)
{
	sf::Vector2f v1 = displacement * ratio;
	sf::Vector2f v2 = displacement * (1 - ratio);
	v2.y = -v2.y;

	return std::make_pair(v1, v2);
}

std::vector<sf::Vector2f> Movement::deflect(sf::FloatRect rect)
{
	std::vector<sf::Vector2f> displacements = { displacement };
	std::pair<float, Side> nextContactRatio = calcNextContactRatio(rect);
	sf::Vector2f currentPosition = origin;

	while (nextContactRatio.first < 1)
	{
		std::pair<sf::Vector2f, sf::Vector2f> vPair;

		switch (nextContactRatio.second)
		{
		case Left:
		case Right:
			vPair = deflectVerticallyAt(nextContactRatio.first);
			break;

		case Top:
		case Bottom:
			vPair = deflectHorizontallyAt(nextContactRatio.first);
			break;
		}

		displacements.pop_back();
		displacements.push_back(vPair.first);
		displacements.push_back(vPair.second);

		currentPosition += vPair.first;
		nextContactRatio = Movement(currentPosition, displacements.back()).calcNextContactRatio(rect);
	}
	return displacements;
}