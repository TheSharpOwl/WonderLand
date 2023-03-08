#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include <string>

namespace wonderland
{
	class Game;

	class Level
	{
	public:
		Level(int levelNumber);

	protected:
		friend class Game;
		const std::string m_levelName;
		sf::Texture m_backgroundTexture;
		sf::Sprite m_backgroundSprite;
	};


}

#endif