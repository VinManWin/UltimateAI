#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
#include "TeamId.hpp"

class Score : public sf::Transformable, public sf::Drawable
{
	unsigned int a, b;
	sf::Time playedTime;
	sf::Text score, time, scoreBg, timeBg;

	void updateScore();

public:
	Score(sf::Font& font);

	void setScore(unsigned int a, unsigned int b);
	void increment(TeamId teamId);

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	void update(sf::Time t);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

