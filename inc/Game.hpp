#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include <cstdint>
#include<memory>

namespace wonderland
{
	class Character;
	class Game
	{
	public:
		Game();

		virtual ~Game() = default;

		void run();

		// todo rename to windowWidth...etc
		static constexpr  float screenWidth = 1024.f;
		static constexpr float screenHeight = 720.f;

	private:

		std::unique_ptr<sf::RenderWindow> m_window;

		// todo find better way to do what this function does

		void loadCharacters();
		// todo check why unique ptr is not working still (( ?!!!!!!!
		// always the character we control is at index zero
		std::vector<std::shared_ptr<Character>> m_characters;
	};
}
#endif