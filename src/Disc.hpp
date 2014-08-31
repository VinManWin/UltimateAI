#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include "MobileObject.hpp"
class Player;

class Disc : public MobileObject
{
public:
	static const float minSpeed, maxSpeed;
	static const float radius;

private:
	sf::CircleShape circle;

	Player* possessingPlayer;
	Player* previouslyPossessingPlayer;

public:
	Disc(sf::Color color);

	void shoot(sf::Vector2f speed);
	void snatch(Player& possessingPlayer);

	sf::Color getColor() const;
	float getRadius() const;

	bool isCaught() const;
	bool wasCaught() const;

	const Player* getPossessingPlayer() const;
	Player* getPossessingPlayer();

	const Player* getPreviouslyPossessingPlayer() const;
	Player* getPreviouslyPossessingPlayer();

	float getMinSpeed() const;
	float getMaxSpeed() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

