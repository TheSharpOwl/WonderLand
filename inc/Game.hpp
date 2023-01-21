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

		virtual ~Game();

		void run();

		// todo rename to windowWidth...etc
		static constexpr  float screenWidth = 1024;
		static constexpr float screenHeight = 720;

	protected:

		std::unique_ptr<sf::RenderWindow> m_window;

		// todo find better way to do what this function does

		void loadCharacters();

		void handleKeyboardEvents(float dt);

		void handleCollisions(float dt) const;

		// todo check why unique ptr is not working still (( ?!!!!!!!
		// always the character we control is at index zero
		std::vector<std::unique_ptr<Character>> m_characters;

		static constexpr int playerIdx = 0;
	};
}
#endif