#include "Match.hpp"

#include <iostream>
#include <sstream>
#include <chrono>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Movement.hpp"
#include "utility.hpp"


Match::Match(sf::RenderWindow& wnd, std::string scriptA, std::string scriptB) :
wnd(wnd),
matchView(wnd.getDefaultView()),
rand(std::chrono::system_clock::now().time_since_epoch().count()),
disc(sf::Color::Red)
{
	playerFont.loadFromFile("res/DejaVuSans.ttf");
	scoreFont.loadFromFile("res/Digital-7.ttf");
	whistleSoundBuffer.loadFromFile("res/whistle.wav");
	whistleSound.setBuffer(whistleSoundBuffer);

	teamA = std::make_unique<Team>(scriptA, TeamId::TeamA, sf::Color::Black, playerFont);
	teamB = std::make_unique<Team>(scriptB, TeamId::TeamB, sf::Color::White, playerFont);

	disc.setPosition(0, 0);

	score = std::make_unique<Score>(scoreFont);
	score->setPosition(0, -(int)field.height / 2 - 9);

	fitViewToWindow();

	defineScriptGlobals();
	teamA->getScript().callGlobalScope();
	teamB->getScript().callGlobalScope();
	resetPlay(KickoffType::Random);
}

void Match::resetPlay(KickoffType type)
{
	state = MatchState::Paused;
	setupFormation(*teamA, true, PlayDirection::toRight);
	setupFormation(*teamB, false, PlayDirection::toLeft);

	if (type == KickoffType::TeamA)
		giveDiscTo(TeamId::TeamA);
	else if (type == KickoffType::TeamB)
		giveDiscTo(TeamId::TeamB);

	startKickoffTimer(type);
}

void Match::giveDiscTo(TeamId teamId)
{
	getTeam(teamId).getPlayer(1).snatch(disc);
}

void Match::startKickoffTimer(KickoffType type)
{
	gameStartTimer.setCallback(std::bind(&Match::kickoff, this, type));
	gameStartTimer.set(sf::milliseconds(1000));
	gameStartTimer.start();
}

void Match::kickoff(KickoffType type)
{
	if (type == KickoffType::Random)
	{
		const int numShotAngles =16;
		std::uniform_int_distribution<int> dist(0, numShotAngles - 1);

		float r = dist(rand);
		float ratio = (2 * r + 1) / (2 * numShotAngles);
		float angle = 2 * Pi * ratio;
		sf::Vector2f initDir(cos(angle), sin(angle));

		disc.shoot(initDir * 10.f);
	}

	captureAll();
	teamA->getScript().start();
	teamB->getScript().start();

	whistleSound.play();

	state = MatchState::Running;
}

void Match::handleEvent(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::Resized:
		fitViewToWindow();
		break;
	}
}

void Match::update(sf::Time t)
{
	gameStartTimer.capture(t);

	if (state == MatchState::Running)
	{
		Movement mov(disc.getPosition(), disc.getDisplacement(t));
		std::vector<sf::Vector2f> travel = mov.deflect(field.getPlayBounds());
		sf::Vector2f displacement;
		for (sf::Vector2f v : travel)
		{
			disc.setVelocity(v * len(disc.getVelocity()) / len(v)); // very dirty; improve later
			disc.move(v);
			displacement += v;
		}

		teamA->movePlayers(t, field.getPlayBounds(), disc.getPossessingPlayer());
		teamB->movePlayers(t, field.getPlayBounds(), disc.getPossessingPlayer());

		std::uniform_int_distribution<int> dist(0, 1);
		float r = dist(rand);
		if (r == 0)
			teamA->getScript().update(t.asSeconds());
		teamB->getScript().update(t.asSeconds());
		if (r == 1)
			teamA->getScript().update(t.asSeconds());


		checkDiscWithinReach();

		score->update(t);
	}
}

void Match::draw()
{
	wnd.setView(matchView);
	wnd.draw(field);
	wnd.draw(*teamA);
	wnd.draw(*teamB);
	wnd.draw(disc);
	wnd.draw(*score);
}

void Match::fitViewToWindow()
{
	sf::FloatRect r = score->getGlobalBounds();
	sf::FloatRect viewRect = combineRects(field.getVisualBounds(), score->getGlobalBounds());
	sf::Vector2f viewSize(wnd.getSize());
	float widthFactor = viewRect.width / viewSize.x;
	float heightFactor = viewRect.height / viewSize.y;

	viewSize *= std::max(widthFactor, heightFactor);

	matchView.setCenter(viewRect.left + 0.5 * viewRect.width, viewRect.top + 0.5 * viewRect.height);
	matchView.setSize(viewSize);
}

void Match::setupFormation(Team& team, bool startingTeam, PlayDirection dir)
{
	std::vector<sf::Vector2f> formationA = team.getScript().getFormation(startingTeam);

	for (size_t i = 0; i < Team::size; ++i)
	{
		sf::Vector2f pos = formationA[i];
		Field::Location l = field.locate(pos);
		if (l != Field::Location::LeftZone)
		{
			std::stringstream s;
			s << "Player " << i + 1 << "'s initial position was not in his zone. Player was reset.";
			team.getScript().log(s.str(), Logfile::Warning);

			pos.x = clamp(pos.x, -(float)field.width / 2, -(float)field.width / 2 + field.endzoneWidth);
			pos.y = clamp(pos.y, -(float)field.height / 2, (float)field.height / 2);
		}
		team.getPlayer(i + 1).setPosition(field.convertToRel(pos, dir));
	}
}

void Match::defineScriptGlobals()
{
	captureAll();

	using namespace std::placeholders;

	scr::Globals g = scr::Globals{ Player::maxSpeed, Player::radius, Disc::minSpeed, Disc::maxSpeed, Disc::radius };
	teamA->getScript().set(g);
	teamB->getScript().set(g);

	teamA->getScript().setShootCallback(std::bind(&Match::onShotRequest, this, TeamId::TeamA, _1, _2));
	teamB->getScript().setShootCallback(std::bind(&Match::onShotRequest, this, TeamId::TeamB, _1, _2));

	teamA->getScript().setSnatchCallback(std::bind(&Match::onSnatchRequest, this, TeamId::TeamA, _1));
	teamB->getScript().setSnatchCallback(std::bind(&Match::onSnatchRequest, this, TeamId::TeamB, _1));

	teamA->getScript().setSetSpeedCallback(std::bind(&Match::onSetSpeedRequest, this, TeamId::TeamA, _1, _2));
	teamB->getScript().setSetSpeedCallback(std::bind(&Match::onSetSpeedRequest, this, TeamId::TeamB, _1, _2));
}

bool Match::onShotRequest(TeamId teamId, unsigned int playerNumber, sf::Vector2f speed)
{
	bool result = false;
	if (playerNumber > 0 && playerNumber <= Team::size)
	{
		if (disc.isCaught())
		{
			if (*disc.getPossessingPlayer() == getTeam(teamId).getPlayer(playerNumber))
			{
				getTeam(teamId).getPlayer(playerNumber).shoot(disc, toFieldCoords(speed, teamId));
				result = true;

				captureAll();
				getOpponent(teamId).getScript().onOpponentShot();
			}
			else
				getTeam(teamId).getScript().log("Tried to shoot with player that does not possess the disc.", Logfile::Warning);
		}
		else
			getTeam(teamId).getScript().log("Tried to shoot a disc that was not caught.", Logfile::Warning);
	}
	else
		getTeam(teamId).getScript().log("Tried to shoot with invalid player.", Logfile::Warning);

	captureAll();
	return result;
}

Field::Location getZoneOf(TeamId id)
{
	return id == TeamId::TeamA ? Field::Location::LeftZone : Field::Location::RightZone;
}

Field::Location getTargetZoneOf(TeamId id)
{
	return getZoneOf(getOpponentId(id));
}

bool Match::onSnatchRequest(TeamId teamId, unsigned int playerNumber)
{
	bool result = false;
	Player& p = getTeam(teamId).getPlayer(playerNumber);

	if (playerNumber > 0 && playerNumber <= Team::size)
	{
		if (!disc.isCaught())
		{
			bool canSnatch = true;
			if (disc.wasCaught())
				canSnatch = canSnatch && *disc.getPreviouslyPossessingPlayer() != p;
			//&& !disc.getPreviouslyPossessingPlayer().canReach(disc);
			// && disc.getPreviouslyPossessingPlayer().getTeamId() != p.getTeamId();

			if (canSnatch)
			{
				p.snatch(disc);
				result = true;

				bool score = field.locate(p.getPosition()) == getTargetZoneOf(teamId);

				getOpponent(teamId).getScript().onOpponentSnatch(score);

				if (score)
				{
					Match::score->increment(teamId);

					switch (teamId)
					{
					case TeamId::TeamA:
						resetPlay(KickoffType::TeamB);
						break;

					case TeamId::TeamB:
						resetPlay(KickoffType::TeamA);
						break;
					}
				}
			}
			else
				getTeam(teamId).getScript().log("Tried to re-snatch.", Logfile::Warning);
		}
		else
			getTeam(teamId).getScript().log("Tried to snatch a disc that was already caught.", Logfile::Warning);
	}
	else
		getTeam(teamId).getScript().log("Tried to snatch with invalid player.", Logfile::Warning);

	captureAll();
	return result;
}

bool Match::onSetSpeedRequest(TeamId teamId, unsigned int playerNumber, sf::Vector2f speed)
{
	bool result = false;
	if (playerNumber > 0 && playerNumber <= Team::size)
	{
		Player& p = getTeam(teamId).getPlayer(playerNumber);

		if (disc.getPossessingPlayer() && p == *disc.getPossessingPlayer())
			getTeam(teamId).getScript().log("Set possessing players' speed.", Logfile::Warning);

		p.setSpeed(toFieldCoords(speed, teamId));
		result = true;
	}
	else
		getTeam(teamId).getScript().log("Tried to set an invalid players' speed.", Logfile::Warning);

	captureAll();
	return result;
}

Team& Match::getTeam(TeamId teamId)
{
	Team* t = nullptr;
	switch (teamId)
	{
	case TeamId::TeamA:
		t = &(*teamA);
		break;
	case TeamId::TeamB:
		t = &(*teamB);
		break;
	}
	return *t;
}

Team& Match::getOpponent(TeamId teamId)
{
	return getTeam(getOpponentId(teamId));
}

TeamId getOpponentId(TeamId teamId)
{
	return (teamId == TeamId::TeamA) ? TeamId::TeamB : TeamId::TeamA;
}


template <typename T>
sf::Vector2<T> Match::toTeamCoords(sf::Vector2<T> v, TeamId t)
{
	if (t == TeamId::TeamB)
		v = -v;

	return v;
}

template <typename T>
sf::Vector2<T> Match::toFieldCoords(sf::Vector2<T> v, TeamId t)
{
	return toTeamCoords(v, t);
}

void Match::captureField()
{
	scr::Field f{ field.width, field.height, field.endzoneWidth, field.width / 2.f, field.height / 2.f, field.width / 2.f - field.endzoneWidth };

	teamA->getScript().set(f);
	teamB->getScript().set(f);
}

void Match::captureTeams()
{
	teamA->getScript().set(scr::Team{ scr::Team::Mine, teamA->getScore() });
	teamA->getScript().set(scr::Team{ scr::Team::Opponent, teamB->getScore() });
	teamB->getScript().set(scr::Team{ scr::Team::Opponent, teamA->getScore() });
	teamB->getScript().set(scr::Team{ scr::Team::Mine, teamB->getScore() });
}

void Match::capturePlayers()
{
	for (size_t i = 0; i < Team::size; ++i)
	{
		size_t num = i + 1;
		Player& pA = teamA->getPlayer(num);
		Player& pB = teamB->getPlayer(num);


		teamA->getScript().set(scr::Player{ scr::Team::Mine, num, pA.getPosition(), pA.getVelocity() });
		teamA->getScript().set(scr::Player{ scr::Team::Opponent, num, pB.getPosition(), pB.getVelocity() });

		teamB->getScript().set(scr::Player{ scr::Team::Opponent, num, -pA.getPosition(), -pA.getVelocity() });
		teamB->getScript().set(scr::Player{ scr::Team::Mine, num, -pB.getPosition(), -pB.getVelocity() });
	}
}

void Match::captureDisc()
{
	if (disc.isCaught())
	{
		Player& p = *disc.getPossessingPlayer();

		teamA->getScript().set(scr::Disc{ true, p.getTeamId() == teamA->getId() ? scr::Team::Mine : scr::Team::Opponent, p.getNumber(), p.getPosition(), p.getVelocity() });
		teamB->getScript().set(scr::Disc{ true, p.getTeamId() == teamB->getId() ? scr::Team::Mine : scr::Team::Opponent, p.getNumber(), -p.getPosition(), -p.getVelocity() });
	}
	else
	{
		teamA->getScript().set(scr::Disc{ false, scr::Team::Opponent, 0, disc.getPosition(), disc.getVelocity() });
		teamB->getScript().set(scr::Disc{ false, scr::Team::Opponent, 0, -disc.getPosition(), -disc.getVelocity() });
	}
}

void Match::captureAll()
{
	captureField();
	captureTeams();
	capturePlayers();
	captureDisc();
}

void Match::checkDiscWithinReach()
{
	if (!disc.isCaught())
	{
		std::vector<Player*> playersWithinReach;

		for (size_t i = 0; i < Team::size; ++i)
		{
			if (teamA->getPlayer(i + 1).canReach(disc) && !(disc.wasCaught() && teamA->getPlayer(i + 1) == *disc.getPreviouslyPossessingPlayer()))
				playersWithinReach.push_back(&teamA->getPlayer(i + 1));

			if (teamB->getPlayer(i + 1).canReach(disc) && !(disc.wasCaught() && teamB->getPlayer(i + 1) == *disc.getPreviouslyPossessingPlayer()))
				playersWithinReach.push_back(&teamB->getPlayer(i + 1));
		}

		if (playersWithinReach.size() > 1)
		{
			Player* closestPlayer = playersWithinReach[0];
			sf::Vector2f closestDistance = disc.getPosition() - closestPlayer->getPosition();
			for (size_t i = 0; i < playersWithinReach.size(); ++i)
			{
				sf::Vector2f dist = disc.getPosition() - playersWithinReach[i]->getPosition();
				if ((closestDistance.x * closestDistance.x + closestDistance.y * closestDistance.y) < (dist.x * dist.x + dist.y * dist.y))
				{
					closestPlayer = playersWithinReach[i];
					closestDistance = dist;
				}
			}
			
			captureAll();
			if (closestPlayer->getTeamId() == TeamId::TeamA)
				teamA->getScript().onDiscWithinReach(closestPlayer->getNumber());
			else
				teamB->getScript().onDiscWithinReach(closestPlayer->getNumber());
		}
		else if (playersWithinReach.size() == 1)
		{
			captureAll();

			Player* p = playersWithinReach[0];
			if (p->getTeamId() == TeamId::TeamA)
				teamA->getScript().onDiscWithinReach(p->getNumber());
			else
				teamB->getScript().onDiscWithinReach(p->getNumber());
		}
	}
}