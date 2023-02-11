#include "Bot.hpp"

namespace wonderland
{
	Bot::Bot(sf::Vector2f pos, std::vector<Animation> animations, int strength) : Character(pos, CharacterType::Enemy, std::move(animations), strength)
	{
		// change the value for the bot
		speed = 100.f;
	}

	void Bot::update(float dt)
	{
		bool playerOnLeft = (m_playerRect.left + m_playerRect.width / 2) < (getCollisionRect().left + getCollisionRect().width/2);
		bool collides = m_playerRect.intersects(getCollisionRect());

		static float lastAttackTime = 0.f;
		static float rightNow = 0.f;
		static int attackAnimationPlay = 0.f;
		rightNow += dt;


		bool canAttack = lastAttackTime + timeBetweenAttacks < rightNow && collides;

		// -1 for left and 1 for right
		auto attack = [this](int direction) {

			if (direction < 0)
				currentAnimationType = AnimationType::AttackLeft;
			else
				currentAnimationType = AnimationType::AttackRight;
			setDirection({ 0,0 });

			attackAnimationPlay++;
			if (attackAnimationPlay == m_attackDurationCount)
			{
				lastAttackTime = rightNow;
				attackAnimationPlay = 0;
			}
		};

		// -1 for left and 1 for right
		auto walk = [this](int direction) {

			if (direction < 0)
				currentAnimationType = AnimationType::WalkingLeft;
			else
				currentAnimationType = AnimationType::WalkingRight;

			setDirection({ static_cast<float>(direction),0 });
		};

		// suppose the player is on the right side
		int direction = 1;
		if (playerOnLeft)
			direction = -1;

		if (canAttack)
			attack(direction);
		else if(collides)
			currentAnimationType = AnimationType::Idle;
		else
			walk(direction);
		 
		Base::update(dt);
	}

	void Bot::updatePlayerRect(sf::Rect<float> playerRect)
	{
		m_playerRect = playerRect;
	}
}
