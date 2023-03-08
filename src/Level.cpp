#include "Level.hpp"
#include "Game.hpp"
#include "Audio.hpp"

#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"

const std::string levelConfigPath = "../config/Levels.json";

namespace wonderland
{
	Level::Level(int levelNumber)
	{

		std::string backgroundImgPath;
		std::string backgroundMusicPath;

		using json = nlohmann::json;

		std::ifstream f(levelConfigPath);
		json data = json::parse(f);

		bool found = false;
		for(const auto &d : data)
		{
			if(d["levelNo"] == levelNumber)
			{
				backgroundImgPath = d["backgroundImage"];
				backgroundMusicPath = d["backgroundMusic"];
				found = true;
				break;
			}
		}

		if(!found)
		{
			std::cout << "error level number " << levelNumber << " not found\n";
			exit(-1);
		}

		if (!m_backgroundTexture.loadFromFile(backgroundImgPath))
		{
			std::cout << "error loading texture " << backgroundImgPath << " for level\n";
			exit(-1);
		}

		// load texture into the sprite
		m_backgroundSprite.setTexture(m_backgroundTexture);
		// scale it to fit the window size
		m_backgroundSprite.setScale(Game::screenWidth / m_backgroundTexture.getSize().x, Game::screenHeight / m_backgroundTexture.getSize().y);

		// todo load music from the parameter
		Audio::startBackgroundMusic();

	}
}

