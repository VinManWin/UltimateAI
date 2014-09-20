#include "Team.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include "utility.hpp"
#include "make_unique.hpp"

Team::Team(std::string file, TeamId id, sf::Color color, sf::Font& playerFont) :
script(file, size),
id(id)
{
	size_t i = 0;
	for (auto& player : players)
	{
		player = unique<Player>(color, id, ++i, playerFont);
	}
}

Player& Team::getPlayer(size_t i)
{
	return *players[i - 1];
}

const Player& Team::getPlayer(size_t i) const
{
	return *players[i];
}

AI& Team::getScript()
{
	return script;
}

const AI& Team::getScript() const
{
	return script;
}

TeamId Team::getId() const
{
	return id;
}

void Team::setScore(unsigned int score)
{
	this->score = score;
}

void Team::incrementScore()
{
	++score;
}

unsigned int Team::getScore() const
{
	return score;
}

void Team::movePlayers(sf::Time t, sf::FloatRect fieldBounds, const Player* possessingPlayer)
{
	for (auto& player : players)
	{
		bool isPossessingPlayer = !possessingPlayer || possessingPlayer && *possessingPlayer != *player;
		if (isPossessingPlayer)
		{
			player->move(player->getDisplacement(t));

			float x = clamp(player->getPosition().x, fieldBounds.left, fieldBounds.left + fieldBounds.width);
			float y = clamp(player->getPosition().y, fieldBounds.top, fieldBounds.top + fieldBounds.height);
			player->setPosition(x, y);

			if (possessingPlayer && *possessingPlayer != *player)
			{
				sf::Vector2f dist = player->getPosition() - possessingPlayer->getPosition();
				float minMarg = player->getRadius() + possessingPlayer->getRadius();
				if (len(dist) < minMarg)
				{
					player->setPosition(possessingPlayer->getPosition() + norm(dist) * minMarg);
				}
			}
		}
	}
}

void Team::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& player : players)
	{
		target.draw(*player, states);
	}
}