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


	void Game::run()
	{
		// todo make vector of characters this one is here for demo
		


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

			sf::Vector2f dir = { 0.0f,0.0f };
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				dir.x = 1.f;
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				dir.x = -1.f;
			}


			m_characters[0]->setDirection(dir);


			// todo this is temp find better solution
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				m_characters[0]->attack(dt);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				m_characters[0]->jump(dt);
			else
				m_characters[0]->update(dt);


			m_window->clear();

			m_window->draw(levelOne.m_backgroundSprite);
			// todo bad but ok for now
			m_characters[0]->draw(*m_window);

			m_window->display();
		}
	}

	void Game::loadCharacters()
	{
		// first character (which the player controls)
		std::vector<Animation> playerCharacterAnim;
		playerCharacterAnim.resize(animationTypeToInt(AnimationType::Count));
		playerCharacterAnim[animationTypeToInt(AnimationType::WalkingRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, { 2.f, 2.f });
		playerCharacterAnim[animationTypeToInt(AnimationType::WalkingLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, { 2.f, 2.f }, true);
		playerCharacterAnim[animationTypeToInt(AnimationType::AttackRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Attack2_6.png", 6, 0.1f, { 2.f, 2.f });
		playerCharacterAnim[animationTypeToInt(AnimationType::AttackLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Attack2_6.png", 6, 0.1f, { 2.f, 2.f }, true);
		playerCharacterAnim[animationTypeToInt(AnimationType::JumpRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Jump_8.png", 8, 0.1f, { 2.f, 2.f });
		playerCharacterAnim[animationTypeToInt(AnimationType::JumpLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Jump_8.png", 8, 0.1f, { 2.f, 2.f }, true);
		playerCharacterAnim[animationTypeToInt(AnimationType::Idle)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Idle_4.png", 4, 0.1f, { 2.f, 2.f });

		auto playerCharacter = std::make_shared<Character>(sf::Vector2f{ 100.f, 500.f }, playerCharacterAnim);
		m_characters.push_back(std::move(playerCharacter));
	}
}
