#include "Game.hpp"
#include "Level.hpp"
#include "Character.hpp"
#include "Bot.hpp"
#include "Math.hpp"
#include "UI.hpp"
#include "Audio.hpp"

#include <chrono>
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
		Level levelOne("Level 1", "../assets/game_background_1.png", "");

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
		
		{
			// first character (which the player controls)
			TextureChangeInfo info;
			info.scale = { 2.f, 2.f };
			info.upCut = 3.f;
			std::vector<Animation> playerCharacterAnim;
			playerCharacterAnim.resize(animationTypeToInt(AnimationType::Count));
			playerCharacterAnim[animationTypeToInt(AnimationType::WalkingRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, info);
			playerCharacterAnim[animationTypeToInt(AnimationType::WalkingLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, info,true);
			playerCharacterAnim[animationTypeToInt(AnimationType::AttackRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Attack2_6.png", 6, 0.1f, info);
			playerCharacterAnim[animationTypeToInt(AnimationType::AttackLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Attack2_6.png", 6, 0.1f, info, true);
			playerCharacterAnim[animationTypeToInt(AnimationType::JumpRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Jump_8.png", 8, 0.1f, info);
			playerCharacterAnim[animationTypeToInt(AnimationType::JumpLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Jump_8.png", 8, 0.1f, info, true);
			playerCharacterAnim[animationTypeToInt(AnimationType::Idle)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Idle_4.png", 4, 0.1f, info);

			const int playerStrength = 25;
			auto playerCharacter = std::make_shared<Character>(sf::Vector2f{ 100.f, 500.f + info.upCut * 2 }, CharacterType::Player, playerCharacterAnim, playerStrength);
			m_characters.push_back(std::move(playerCharacter));
		}

		// todo make a pointer to animation then if null don't do it later or use optional
		std::vector<Animation> enemyCharacterAnim;
		// todo might need to leave more space for animations to not look weird
		{
			TextureChangeInfo info;
			info.leftCut = 35.f;
			info.upCut = 60.f;
			info.rightCut = 35.f;
			info.scale = { 2.f, 2.f };

			enemyCharacterAnim.resize(animationTypeToInt(AnimationType::Count));
			enemyCharacterAnim[animationTypeToInt(AnimationType::WalkingRight)] = Animation("../assets/Skeleton_Warrior/Walk.png", 7, 0.1f, info, false);
			enemyCharacterAnim[animationTypeToInt(AnimationType::WalkingLeft)] = Animation("../assets/Skeleton_Warrior/Walk.png", 7, 0.1f, info, true);
			enemyCharacterAnim[animationTypeToInt(AnimationType::AttackRight)] = Animation("../assets/Skeleton_Warrior/Attack_3.png", 4, 0.1f, info, false);
			enemyCharacterAnim[animationTypeToInt(AnimationType::AttackLeft)] = Animation("../assets/Skeleton_Warrior/Attack_3.png", 4, 0.1f, info, true);
			enemyCharacterAnim[animationTypeToInt(AnimationType::JumpRight)] = {}; // todo lol enemies cannot jump (I think as a game design it is fine just wanna improve it some time xD)
			enemyCharacterAnim[animationTypeToInt(AnimationType::JumpLeft)] = {};
			enemyCharacterAnim[animationTypeToInt(AnimationType::Idle)] = Animation("../assets/Skeleton_Warrior/Idle.png", 7, 0.1f, info, true);

			const int enemyStrength = 10;
			auto enemyCharacter = std::make_shared<Bot>(sf::Vector2f{ 700.f, 309 + info.upCut * 2 }, enemyCharacterAnim, enemyStrength);
			m_characters.push_back(std::move(enemyCharacter));
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
			m_characters[a]->getPoints(1); // todo random number

		for (auto d : damaged)
			m_characters[d.first]->getDamage(d.second);

		counter++;
	}
}
