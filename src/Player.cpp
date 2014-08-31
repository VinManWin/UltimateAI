#include "Player.hpp"

#include <string>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Disc.hpp"
#include "utility.hpp"

const float Player::maxSpeed = 6.f;
const float Player::radius = 1.f;

Player::Player(sf::Color color, TeamId team, unsigned int number, const sf::Font& font) :
color(color),
team(team),
number(number)
{

	sf::Color fillColor(color);
	circle.setFillColor(fillColor);
	circle.setRadius(Disc::radius);

	circle.setOutlineColor(color);
	circle.setOutlineThickness(radius - Disc::radius);

	sf::FloatRect bounds = circle.getLocalBounds();
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	std::string numberStr = std::to_string(number);
	text.setFont(font);
	text.setString(numberStr);
	text.setScale(0.07f, 0.07f);
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.width * 0.5f, textBounds.height * 0.5f);
	text.setPosition(0, -0.5f);
	text.setColor(sf::Color(255 - color.r, 255 - color.g, 255 - color.b, 255));
}

void Player::shoot(Disc& disc, sf::Vector2f speed)
{
	disc.setPosition(getPosition() + norm(speed) * (radius + Disc::radius));
	disc.shoot(speed);
	circle.setFillColor(color);
}

void Player::snatch(Disc& disc)
{
	if (disc.isCaught())
	{
		Player& p = *disc.getPossessingPlayer();
		p.circle.setFillColor(p.color);
	}

	disc.setPosition(getPosition());
	disc.snatch(*this);
	circle.setFillColor(disc.getColor());
}

float Player::getRadius() const
{
	return radius;
}

unsigned int Player::getNumber() const
{
	return number;
}

TeamId Player::getTeamId() const
{
	return team;
}

bool Player::canReach(const Disc& d) const
{
	sf::Vector2f distV = d.getPosition() - getPosition();
	return (distV.x * distV.x + distV.y * distV.y) <= (d.getRadius() + getRadius()) * (d.getRadius() + getRadius());
}

void Player::setSpeed(sf::Vector2f v)
{
	float length = clamp(len(v), 0.f, getMaxSpeed());

	MobileObject::setSpeed(norm(v) * length);
}

float Player::getMaxSpeed() const
{
	return maxSpeed;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(circle, states);
	target.draw(text, states);
}

bool operator==(const Player& a, const Player& b)
{
	return a.getNumber() == b.getNumber() && a.getTeamId() == b.getTeamId();
}

bool operator!=(const Player& a, const Player& b)
{
	return !(a == b);
}