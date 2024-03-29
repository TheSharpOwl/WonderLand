#include "Character.hpp"

#include <algorithm>
// for debugging strings

// todo define the macro from cmake (in case it's debug mode)
#ifdef DEBUG_PHYSICS
#include "SFML/Graphics.hpp"
#endif

namespace wonderland
{
	Character::Character(sf::Vector2f pos, CharacterType type, std::vector<Animation> animations, int strength) : m_startPos(pos),
																									m_type(type),
																									m_animations(std::move(animations)),
																									m_hp(100), m_strength(strength)
	{
		m_pos = m_startPos;
	}

	void Character::draw(sf::RenderTarget &rt) const
	{
		rt.draw(m_sprite);
		// todo add such macros to wiki or readme
		// todo change color when character collides with another
		// todo seems custom size for collision is needed (because skeleton box it too big) or seems something else for skeleton because without scale it looks same size but wrong pos
#ifdef DEBUG_PHYSICS
			sf::RectangleShape rect(sf::Vector2f(m_sprite.getTextureRect().width * m_sprite.getScale().x, m_sprite.getTextureRect().height * m_sprite.getScale().y));
			rect.setPosition(m_sprite.getPosition());
			rect.setOutlineThickness(1.f);
			rect.setFillColor(sf::Color(0.f, 0.f, 0.f, 0.f));

			if(m_isGettingDamage)
				rect.setOutlineColor(sf::Color(255, 0, 0)); // red
			else if(m_isGettingPoints)
				rect.setOutlineColor(sf::Color(2240, 230, 140)); // yellow
			else
				rect.setOutlineColor(sf::Color(127, 255, 0)); // green

			rt.draw(rect);
#endif
	}

		void Character::setDirection(sf::Vector2f di)
		{
			// y increases from up to down so we add gravity instead of discarding
			m_vel.y += m_gravity;
			m_vel.x = speed * di.x;
		}

		void Character::update(float dt)
		{
			if(!isAttacking()) // if attack set the state already we don't change it
			{
				if (m_vel.y < 0) // we are jumping
				{
					if (m_vel.x < 0.f)
						currentAnimationType = AnimationType::JumpLeft;
					else
						currentAnimationType = AnimationType::JumpRight;
				}
				else // we are on the ground
				{
					if (m_vel.x > 0.f)
						currentAnimationType = AnimationType::WalkingRight;
					else if (m_vel.x < 0.f)
						currentAnimationType = AnimationType::WalkingLeft;
					else
						currentAnimationType = AnimationType::Idle;
				}
			}
			m_pos += m_vel * dt;
			// todo make it ground not start pos and static const for example or something in Level and take it from there
			if(m_pos.y > m_startPos.y)
			{
				m_pos.y = m_startPos.y;
				m_vel.y = 0.f;
			}

			auto const iAnimation = animationTypeToInt(currentAnimationType);
			m_animations[iAnimation].update(dt);
			m_animations[iAnimation].applyToSprite(m_sprite);
			m_sprite.setPosition(m_pos);
		}
		// todo maybe apply gravity even while attacking?
		void Character::attack(float dt)
		{
			if (m_vel.x < 0.f)
				currentAnimationType = AnimationType::AttackLeft;
			else // because by default our character looks at right 
				currentAnimationType = AnimationType::AttackRight;

			m_vel.y = m_vel.x = 0.f; // no movement while attacking
		}

		void Character::jump(float dt)
		{
			// negative because sfml y starts from 0 up and increases down
			if(m_pos.y == m_startPos.y) // to prevent infinite jumping
				m_vel.y = -350.f; // todo this is temp
		}

		void Character::reset()
		{
			m_isGettingDamage = false;
			m_isGettingPoints = false;
			currentAnimationType = AnimationType::Idle;
		}

		sf::Rect<float> Character::getCollisionRect() const
		{
			sf::RectangleShape rect(sf::Vector2f(m_sprite.getTextureRect().width * m_sprite.getScale().x, m_sprite.getTextureRect().height * m_sprite.getScale().y));
			rect.setPosition(m_sprite.getPosition());

			return rect.getGlobalBounds();
		}

		bool Character::isAttacking() const
		{
			return currentAnimationType == AnimationType::AttackLeft || currentAnimationType == AnimationType::AttackRight;
		}

		void Character::increasePoints(int points)
		{
			m_isGettingPoints = true;
			m_points += points;
		}

		void Character::getDamage(int dmg)
		{
			m_isGettingDamage = true;
			m_hp = std::max(0, m_hp-dmg);
		}

		int Character::getHp() const
		{
			return m_hp;
		}
}
