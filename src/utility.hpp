#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>


const double Pi = 3.141592653589793238463;

template <typename T>
sf::Rect<T> combineRects(sf::Rect<T> r1, sf::Rect<T> r2)
{
	sf::Rect<T> result;
	result.left = std::min(r1.left, r2.left);
	result.top = std::min(r1.top, r2.top);
	result.width = std::max(r1.left + r1.width, r2.left + r2.width) - result.left;
	result.height = std::max(r1.top + r1.height, r2.top + r2.height) - result.top;

	return result;
}

template <typename T>
sf::Rect<T> transformRect(const sf::Rect<T>& r, const sf::Transform& t)
{
	sf::Vector2<T> topLeft(r.left, r.top);
	sf::Vector2<T> bottomRight(r.left + r.width, r.top + r.height);

	topLeft = t * topLeft;
	bottomRight = t * bottomRight;

	return sf::Rect<T>(topLeft, bottomRight - topLeft);
}

template<typename T>
T clamp(const T& val, const T& min, const T& max)
{
	return std::max(min, std::min(max, val));
}

template <typename T>
T len(sf::Vector2<T> v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

template <typename T>
sf::Vector2<T> norm(sf::Vector2<T> v)
{
	return v / (len(v) + 2E-15f);
}

int divRem(int& ds, int dd);