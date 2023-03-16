#include "Game.hpp"
#include "Level.hpp"
#include "Character.hpp"
#include "Bot.hpp"
#include "Math.hpp"
#include "UI.hpp"
#include "Audio.hpp"

#include "nlohmann/json.hpp"

#include <chrono>
#include <fstream>
#include <memory>
#include <utility>

namespace wonderland {
	Game::Game()
	{
		m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(screenWidth, screenHeight), "Wonderland");
		loadCharacters();
	}

	// this is here empty and not = default because the compiler in default case will try to generate one
	// -> won't know where is character dtor (because character class was forward declared)
	// -> so solution is either make characters shared pointers
	// (because seems shared ptr has some indirection for looking up of the dtor) or do this thing and I left this thing for educating myself )
	Game::~Game() 
	{
		
	}

	void Game::run()
	{
		// time point
		auto tp = std::chrono::steady_clock::now();

		// todo maybe dependency design is wrong
		// Todo add background music here and add it in level
		Level levelOne(1);

		while (m_window->isOpen())
		{
			m_restarted = false;

			sf::Event event;
			while (m_window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_window->close();
			}

			float dt;
			{
				const auto new_tp = std::chrono::steady_clock::now();
				dt = std::chrono::duration<float>(new_tp - tp).count();
				tp = new_tp;
			}

			if (m_characters[playerIdx]->getHp() == 0)
			{
				restart();
				// no need to set m_restarted to true
				continue;
			}

			if(m_runGame)
			{
				m_characters[playerIdx]->reset();
				handleKeyboardEvents(dt);

				for (std::size_t i = 1; i < m_characters.size(); i++)
				{
					auto pBot = std::dynamic_pointer_cast<Bot>(m_characters[i]);
					if (pBot->getHp() == 0)
					{
						restart();
						break;
					}
					pBot->reset();
					pBot->updatePlayerRect(m_characters[playerIdx]->getCollisionRect());
					pBot->update(dt);
				}

				if (m_restarted)
					continue;
				handleCollisions(dt);

			}
			else
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					m_runGame = true;
					continue;
				}
			}

			UI::setScore(m_characters[playerIdx]->getPoints());

			m_window->clear();

			m_window->draw(levelOne.m_backgroundSprite);

			// todo bad *m_window but ok for now
			for (std::size_t i = 1; i < m_characters.size(); i++)
			{
				m_characters[i]->draw(*m_window);
			}

			// draw player in the end to appear above every other character in case of overlapping
			if(m_runGame)
			{
				m_characters[playerIdx]->draw(*m_window);

				UI::draw(*m_window, m_characters, playerIdx);
			}
			else
			{
				UI::showControls(*m_window);
			}

			m_window->display();
		}
	}

	void Game::restart()
	{
		m_restarted = true;
		m_runGame = false;
		m_characters.clear();
		// todo reset other stuff here too
		loadCharacters();
	}

	void Game::loadCharacters()
	{
		const std::string configPath = "../config/Animations.json";
		using json = nlohmann::json;

		std::ifstream f(configPath);
		json data = json::parse(f);

		auto stringToAnimationInt = [](const std::string& typeName)
		{
			AnimationType type;
			if (typeName == "WalkingRight")
				type = AnimationType::WalkingRight;
			else if (typeName == "WalkingLeft")
				type = AnimationType::WalkingLeft;
			else if (typeName == "AttackRight")
				type = AnimationType::AttackRight;
			else if (typeName == "AttackLeft")
				type = AnimationType::AttackLeft;
			else if (typeName == "JumpRight")
				type = AnimationType::JumpRight;
			else if (typeName == "JumpLeft")
				type = AnimationType::JumpLeft;
			else if (typeName == "Idle")
				type = AnimationType::Idle;
			else
				return -1;

			return animationTypeToInt(type);

		};
		// first index is the player character always
		for(const auto& d : data)
		{
			std::vector<Animation> animations;
			animations.resize(animationTypeToInt(AnimationType::Count));
			const auto startPos = sf::Vector2f{ d["startPos"][0], d["startPos"][1] };
			const int strength = d["strength"];
			CharacterType characterType = CharacterType::Enemy;
			if (d["name"] == "player")
				characterType = CharacterType::Player;

			for(const auto& a : d["animations"])
			{
				const int idx = stringToAnimationInt(a["type"]);

				if(idx < 0)
				{
					std::cout << "error in the animation type name of " << d["name"] << "\n";
					exit(-1);
				}

				TextureChangeInfo info;

				const std::string filePath = a["file"];
				const int nFrames = a["nFrames"];
				const float holdTime = a["holdTime"];
				const bool reverseX = a["reverse"][0];
				[[maybe_unused]] const bool reverseY = a["reverse"][1];

				info.scale.x = a["scale"][0];
				info.scale.y = a["scale"][1];
				// clockwise order starting from up
				info.upCut = a["cut"][0];
				info.rightCut = a["cut"][1];
				info.downCut = a["cut"][2];
				info.leftCut = a["cut"][3];

				// reverseY is not needed but ok
				animations[idx] = Animation(filePath, nFrames, holdTime, info, reverseX);
			}


			if (characterType == CharacterType::Player)
			{
				auto character = std::make_shared<Character>(startPos, characterType, animations, strength);
				m_characters.push_back(std::move(character));
			}
			else
			{
				auto enemyCharacter = std::make_shared<Bot>(startPos, animations, strength);
				m_characters.push_back(std::move(enemyCharacter));
			}
		}

	}

	void Game::handleKeyboardEvents(float dt)
	{
		sf::Vector2f dir = { 0.0f,0.0f };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			dir.x = 1.f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			dir.x = -1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			restart();
		}

		m_characters[playerIdx]->setDirection(dir);

		// todo this is temp find better solution
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			m_characters[playerIdx]->attack(dt);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			m_characters[playerIdx]->jump(dt);

		m_characters[playerIdx]->update(dt);
	}

	void Game::handleCollisions(float dt) const
	{
		std::vector<size_t> attackers;
		// pair <index of damaged guy, how much damage>
		std::vector<std::pair<size_t, int>> damaged;

		static int64_t counter = 1;

		counter %= 100000000LL;// to avoid overflow I think it's useless but ok better safe than suffering with debugging lol

		for(size_t i = 0;i < m_characters.size();i++)
		{
			for(size_t j = i + 1; j < m_characters.size();j++)
			{
				// todo try removing namespace wonderland
				if(Wonderland::Math::isOverlapping(m_characters[i]->getCollisionRect(), m_characters[j]->getCollisionRect()))
				{
					// TODO later make it separeate attack left or attack right (direction of where the character is attacking)
					if (m_characters[i]->isAttacking() && m_characters[i]->isAnimationFinished())
					{
						attackers.push_back(i);
						damaged.push_back(std::make_pair( j, m_characters[i]->getStrength()));
					}
					if(m_characters[j]->isAttacking() && m_characters[j]->isAnimationFinished())
					{
						attackers.push_back(j);
						damaged.push_back(std::make_pair( i, m_characters[j]->getStrength() ));
					}
				}
			}
		}

		for (auto a : attackers)
			m_characters[a]->increasePoints(1); // todo random number

		for (auto d : damaged)
			m_characters[d.first]->getDamage(d.second);

		counter++;
	}
}
