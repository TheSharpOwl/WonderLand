#include "Game.hpp"
#include "Level.hpp"
#include "Character.hpp"

#include <chrono>

namespace wonderland {
	Game::Game()
	{
		m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(screenWidth, screenHeight), "Wonderland");

	}


	void Game::run()
	{
		// todo make vector of characters this one is here for demo
		Character wizard({ 100.f, 100.f });


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

			wizard.setDirection(dir);
			wizard.update(dt);

			m_window->clear();

			m_window->draw(levelOne.m_backgroundSprite);
			// todo bad but ok for now
			wizard.draw(*m_window);

			m_window->display();
		}
	}
}