#include "Bot.hpp"

namespace wonderland
{
	Bot::Bot(sf::Vector2f pos, std::vector<Animation> animations) : Character(pos, CharacterType::Enemy, std::move(animations))
	{

	}

	void Bot::update(float dt)
	{
		bool playerOnLeft = (m_playerRect.left + m_playerRect.width / 2) < (getCollisionRect().left + getCollisionRect().width/2);
		bool collides = m_playerRect.intersects(getCollisionRect());

		if(playerOnLeft)
		{
			if (collides)
			{
				currentAnimationType = AnimationType::AttackLeft;
				setDirection({ 0,0 });
			}
			else
			{
				currentAnimationType = AnimationType::WalkingLeft;
				setDirection({ -1,0 });
			}
		}
		else
		{
			if (collides)
			{
				currentAnimationType = AnimationType::AttackRight;
				setDirection({ 0,0 });
			}
			else
			{
				currentAnimationType = AnimationType::WalkingRight;
				setDirection({ 1,0 });
			}
		}

		Base::update(dt);
	}

	void Bot::updatePlayerRect(sf::Rect<float> playerRect)
	{
		m_playerRect = playerRect;
	}
}
