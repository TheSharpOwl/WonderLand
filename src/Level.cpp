#include "Level.hpp"

#include "Audio.hpp"
#include <iostream>

#include "Game.hpp"

namespace wonderland
{
	Level::Level(std::string levelName, const std::string& backgroundPath, const std::string& backgroundMusicPath) : m_levelName(std::move(levelName))
	{
		if (!m_backgroundTexture.loadFromFile(backgroundPath))
		{
			std::cout << "error loading texture\n";
			return;
		}

		// load texture into the sprite
		m_backgroundSprite.setTexture(m_backgroundTexture);
		// scale it to fit the window size
		m_backgroundSprite.setScale(Game::screenWidth / m_backgroundTexture.getSize().x, Game::screenHeight / m_backgroundTexture.getSize().y);

		// todo load music from the parameter
		Audio::startBackgroundMusic();

	}
}

