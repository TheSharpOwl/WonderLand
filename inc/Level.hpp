#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

#include <string>

namespace wonderland
{
	class Game;
	// TODO Ziad make levels have numbers for backgrounds and music for example format level_X_background.png and so on...
	class Level
	{
	public:
		Level(std::string levelName, const std::string& backgroundPath, const std::string& backgroundMusicPath);

	protected:
		friend class Game;
		const std::string m_levelName;
		sf::Texture m_backgroundTexture;
		sf::Sprite m_backgroundSprite;
	};


}

#endif