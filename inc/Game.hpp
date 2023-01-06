#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include <cstdint>
#include<memory>

namespace wonderland
{
	class Game
	{
	public:
		Game();

		void run();

		// todo rename to windowWidth...etc
		static constexpr  float screenWidth = 1024.f;
		static constexpr float screenHeight = 720.f;

	private:

		std::unique_ptr<sf::RenderWindow> m_window;
	};
}
#endif