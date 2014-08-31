#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "TeamId.hpp"
#include "MobileObject.hpp"
class Disc;

class Player : public MobileObject
{
public:
	static const float maxSpeed;
	static const float radius;

private:
	sf::CircleShape circle;
	const sf::Color color;

	sf::Text text;

	const unsigned int number;
	const TeamId team;

public:
	Player(sf::Color color, TeamId team, unsigned int number, const sf::Font& font);

	void shoot(Disc& disc, sf::Vector2f speed);
	void snatch(Disc& disc);

	unsigned int getNumber() const;
	TeamId getTeamId() const;

	bool canReach(const Disc& d) const;
	float getRadius() const;

	void setSpeed(sf::Vector2f v);
	float getMaxSpeed() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

bool operator==(const Player& a, const Player& b);
bool operator!=(const Player& a, const Player& b);
