#include "Game.hpp"
#include "Level.hpp"
#include "Character.hpp"

#include <chrono>
#include <memory>

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


			handleKeyboardEvents(dt);

			for(std::size_t i = 1 ; i < m_characters.size();i++)
			{
				m_characters[i]->update(dt);
			}

			m_window->clear();

			m_window->draw(levelOne.m_backgroundSprite);

			// todo bad *m_window but ok for now
			for (std::size_t i = 1; i < m_characters.size(); i++)
			{
				m_characters[i]->draw(*m_window);
			}

			// draw player in the end to appear above every other character in case of overlapping
			m_characters[playerIdx]->draw(*m_window);

			m_window->display();
		}
	}

	void Game::loadCharacters()
	{
		
		{
			// first character (which the player controls)
			TextureChangeInfo info;
			info.scale = { 2.f, 2.f };
			std::vector<Animation> playerCharacterAnim;
			playerCharacterAnim.resize(animationTypeToInt(AnimationType::Count));
			playerCharacterAnim[animationTypeToInt(AnimationType::WalkingRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, info);
			playerCharacterAnim[animationTypeToInt(AnimationType::WalkingLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, info,true);
			playerCharacterAnim[animationTypeToInt(AnimationType::AttackRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Attack2_6.png", 6, 0.1f, info);
			playerCharacterAnim[animationTypeToInt(AnimationType::AttackLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Attack2_6.png", 6, 0.1f, info, true);
			playerCharacterAnim[animationTypeToInt(AnimationType::JumpRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Jump_8.png", 8, 0.1f, info);
			playerCharacterAnim[animationTypeToInt(AnimationType::JumpLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Jump_8.png", 8, 0.1f, info, true);
			playerCharacterAnim[animationTypeToInt(AnimationType::Idle)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Idle_4.png", 4, 0.1f, info);

			auto playerCharacter = std::make_unique<Character>(sf::Vector2f{ 100.f, 500.f }, playerCharacterAnim);
			m_characters.push_back(std::move(playerCharacter));
		}

		// todo make a pointer to animation then if null don't do it later or use optional
		std::vector<Animation> enemyCharacterAnim;
		// todo make cutoff 4 parameters from the 4 sides
		// todo might need to leave more space for animations to not look weird
		{
			TextureChangeInfo info;
			info.leftCut = 35.f;
			info.upCut = 70.f;
			info.scale = { 2.f, 2.f };

			enemyCharacterAnim.resize(animationTypeToInt(AnimationType::Count));
			enemyCharacterAnim[animationTypeToInt(AnimationType::WalkingRight)] = Animation("../assets/Skeleton_Warrior/Walk.png", 7, 0.1f, info, false);
			enemyCharacterAnim[animationTypeToInt(AnimationType::WalkingLeft)] = Animation("../assets/Skeleton_Warrior/Walk.png", 7, 0.1f, info, true);
			enemyCharacterAnim[animationTypeToInt(AnimationType::AttackRight)] = Animation("../assets/Skeleton_Warrior/Attack_3.png", 4, 0.1f, info, false);
			enemyCharacterAnim[animationTypeToInt(AnimationType::AttackLeft)] = Animation("../assets/Skeleton_Warrior/Attack_3.png", 4, 0.1f, info, true);
			enemyCharacterAnim[animationTypeToInt(AnimationType::JumpRight)] = {};
			enemyCharacterAnim[animationTypeToInt(AnimationType::JumpLeft)] = {};
			enemyCharacterAnim[animationTypeToInt(AnimationType::Idle)] = Animation("../assets/Skeleton_Warrior/Idle.png", 7, 0.1f, info, true);

			auto enemyCharacter = std::make_unique<Character>(sf::Vector2f{ 700.f, 309 + info.upCut * 2 }, enemyCharacterAnim);
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


		m_characters[playerIdx]->setDirection(dir);


		// todo this is temp find better solution
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			m_characters[playerIdx]->attack(dt);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			m_characters[playerIdx]->jump(dt);
		else
			m_characters[playerIdx]->update(dt);
	}
}
