#include "Bot.hpp"

namespace wonderland
{
	Bot::Bot(sf::Vector2f pos, std::vector<Animation> animations) : Character(pos, CharacterType::Enemy, std::move(animations))
	{

	}

	void Bot::update(float dt)
	{

	}
}
