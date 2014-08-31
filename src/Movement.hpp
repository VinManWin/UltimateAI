#pragma once

#include <vector>
#include <tuple>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

class Movement
{
	enum Side { Left, Top, Right, Bottom };

	sf::Vector2f origin;
	sf::Vector2f displacement;

	float contactVertically(float barrier);
	float contactHorizontally(float barrier);
	std::pair<float, Side> calcNextContactRatio(sf::FloatRect rect);
	void addDeflection(std::vector<sf::Vector2f> displacements);
	
	std::pair<sf::Vector2f, sf::Vector2f> deflectVerticallyAt(float ratio);
	std::pair<sf::Vector2f, sf::Vector2f> deflectHorizontallyAt(float ratio);

public:
	Movement(sf::Vector2f origin, sf::Vector2f displacement);

	std::vector<sf::Vector2f> deflect(sf::FloatRect rect);
};

