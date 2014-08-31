#pragma once

#include <string>
#include <memory>
#include <random>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Field.hpp"
#include "Disc.hpp"
#include "Team.hpp"
#include "Score.hpp"
#include "Timer.hpp"

namespace sf { class RenderWindow;  }

class Match
{
	enum class MatchState
	{
		Paused,
		Running
	} state;

	sf::RenderWindow& wnd;
	Timer gameStartTimer;
	std::default_random_engine rand;

	sf::Font playerFont;
	sf::Font scoreFont;
	sf::SoundBuffer whistleSoundBuffer;
	sf::Sound whistleSound;

	Field field;
	Disc disc;
	std::unique_ptr<Team> teamA;
	std::unique_ptr<Team> teamB;
	std::unique_ptr<Score> score;

	sf::View matchView;


	enum class KickoffType
	{
		Random,
		TeamA,
		TeamB
	};

	void resetPlay(KickoffType type);
	void startKickoffTimer(KickoffType type);
	void kickoff(KickoffType type);
	void giveDiscTo(TeamId teamId);

	void fitViewToWindow();
	void defineScriptGlobals();
	void setupFormation(Team& team, bool startingTeam, PlayDirection dir);

	bool onShotRequest(TeamId teamId, unsigned int playerNumber, sf::Vector2f speed);
	bool onSnatchRequest(TeamId teamId, unsigned int playerNumber);
	bool onSetSpeedRequest(TeamId teamId, unsigned int playerNumber, sf::Vector2f speed);

	Team& getTeam(TeamId teamId);
	Team& getOpponent(TeamId teamId);

	template <typename T>
	sf::Vector2<T> toTeamCoords(sf::Vector2<T> v, TeamId t);
	template <typename T>
	sf::Vector2<T> toFieldCoords(sf::Vector2<T> v, TeamId t);

	void captureField();
	void captureTeams();
	void capturePlayers();
	void captureDisc();
	void captureAll();

	void checkDiscWithinReach();

public:
	Match(sf::RenderWindow& wnd, std::string scriptA, std::string scriptB);

	void handleEvent(sf::Event e);
	void update(sf::Time t);
	void draw();
};

TeamId getOpponentId(TeamId teamId);
