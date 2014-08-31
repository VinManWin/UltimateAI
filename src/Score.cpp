#include "Score.hpp"

#include <sstream>
#include <iomanip>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "utility.hpp"

void Score::updateScore()
{
	std::stringstream s;
	s << std::setw(1) << std::setfill(' ') << a << ":";
	s << std::setw(1) << std::setfill(' ') << b;
	score.setString(s.str());

	s.str("");
	int ms = playedTime.asMilliseconds();
	s << std::setw(2) << std::setfill('0') << divRem(ms, 60000) << ":";
	s << std::setw(2) << std::setfill('0') << divRem(ms, 1000);
	time.setString(s.str());

	sf::Vector2f scale(0.1, 0.1);
	
	score.setScale(scale);
	sf::FloatRect scoreBounds = score.getLocalBounds();
	score.setOrigin(scoreBounds.width / 2, 0);

	time.setScale(scale);
	sf::FloatRect timeBounds = time.getLocalBounds();
	time.setOrigin(timeBounds.width / 2, timeBounds.height);

	scoreBg.setScale(scale);
	sf::FloatRect scoreBgBounds = scoreBg.getLocalBounds();
	scoreBg.setOrigin(scoreBgBounds.width / 2, 0);
	scoreBg.setColor(sf::Color(255, 255, 255, 50));

	timeBg.setScale(scale);
	sf::FloatRect timeBgBounds = timeBg.getLocalBounds();
	timeBg.setOrigin(timeBgBounds.width / 2, timeBgBounds.height);
	timeBg.setColor(sf::Color(255, 255, 255, 50));
}

Score::Score(sf::Font& font) :
a(0),
b(0),
score("", font, 60),
time("", font, 40),
scoreBg("88:88", font, 60),
timeBg("88:88", font, 40)
{
	updateScore();
}

void Score::setScore(unsigned int a, unsigned int b)
{
	this->a = a;
	this->b = b;
	updateScore();
}

void Score::increment(TeamId teamId)
{
	switch (teamId)
	{
	case TeamId::TeamA:
		++a;
		break;
	case TeamId::TeamB:
		++b;
		break;
	}
	updateScore();
}

sf::FloatRect Score::getLocalBounds() const
{
	return combineRects(time.getGlobalBounds(), score.getGlobalBounds());
	//return sf::FloatRect(getPosition() - getOrigin(), getSize());
}

sf::FloatRect Score::getGlobalBounds() const
{
	return transformRect(getLocalBounds(), getTransform());
}

void Score::update(sf::Time t)
{
	playedTime += t;
	updateScore();
}

void Score::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(scoreBg, states);
	target.draw(timeBg, states);
	target.draw(score, states);
	target.draw(time, states);
}