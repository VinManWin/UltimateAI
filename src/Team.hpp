#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include "Player.hpp"
#include "AI.hpp"
#include "TeamId.hpp"
namespace sf{ class Time; }

class Team : public sf::Drawable
{
public:
	static const unsigned int size = 7;

private:
	std::array<std::unique_ptr<Player>, size> players;
	AI script;
	unsigned int score;

	const TeamId id;

public:
	Team(std::string file, TeamId id, sf::Color color, sf::Font& playerFont);
	Player& getPlayer(size_t i);
	const Player& getPlayer(size_t i) const;

	AI& getScript();
	const AI& getScript() const;

	TeamId getId() const;

	void setScore(unsigned int score);
	void incrementScore();
	unsigned int getScore() const;

	void movePlayers(sf::Time t, sf::FloatRect fieldBounds, const Player* possessingPlayer);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

