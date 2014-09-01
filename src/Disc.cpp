#include "Disc.hpp"

#include <stdexcept>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Player.hpp"
#include "utility.hpp"


const float Disc::minSpeed = 11.f;
const float Disc::maxSpeed = 20.f;
const float Disc::radius = 0.4f;

Disc::Disc(sf::Color color) :
possessingPlayer(nullptr),
previouslyPossessingPlayer(nullptr)
{
	circle.setFillColor(color);
	circle.setRadius(radius);
	circle.setOrigin(radius, radius);
}

void Disc::shoot(sf::Vector2f velocity)
{
	previouslyPossessingPlayer = possessingPlayer;
	possessingPlayer = nullptr;

	float length = clamp(len(velocity), getMinSpeed(), getMaxSpeed());

	setVelocity(norm(velocity) * length);
}

void Disc::snatch(Player& possessingPlayer)
{
	previouslyPossessingPlayer = nullptr;
	this->possessingPlayer = &possessingPlayer;

	setPosition(possessingPlayer.getPosition());
	setVelocity(sf::Vector2f(0, 0));
}

sf::Color Disc::getColor() const
{
	return circle.getFillColor();
}

float Disc::getRadius() const
{
	return radius;
}

bool Disc::isCaught() const
{
	return possessingPlayer != nullptr;
}

bool Disc::wasCaught() const
{
	return previouslyPossessingPlayer != nullptr;
}

const Player* Disc::getPossessingPlayer() const
{
	return possessingPlayer;
}

Player* Disc::getPossessingPlayer()
{
	return possessingPlayer;
}

const Player* Disc::getPreviouslyPossessingPlayer() const
{
	return previouslyPossessingPlayer;
}

Player* Disc::getPreviouslyPossessingPlayer()
{
	return previouslyPossessingPlayer;
}

float Disc::getMinSpeed() const
{
	return minSpeed;
}

float Disc::getMaxSpeed() const
{
	return maxSpeed;
}

void Disc::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	if (!possessingPlayer)
		target.draw(circle, states);
	
}