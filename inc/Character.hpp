#ifndef CHARACTER_H
#define CHARACTER_H

#include "Animation.hpp"

#include <algorithm>

// for debugging strings
#include <Windows.h>

#ifdef DEBUG_PHYSICS
#include "SFML/Graphics.hpp"
#endif

/*
 * with help of:
 * https://github.com/planetchili/sfml_sprite/blob/master/SFMLspriteAnim/Source.cpp
 */

namespace wonderland
{
	enum class CharacterType
	{
		Player,
		Enemy,
		Count
	};

	class Character
	{
	public:
		// todo support adding a character using json file (need nlohman::json for that)
		Character(sf::Vector2f pos, CharacterType type, std::vector<Animation> animations) : m_startPos(pos), m_type(type), m_animations(std::move(animations))
		{
			m_pos = m_startPos;
		}

		virtual ~Character() = default;

		void draw(sf::RenderTarget& rt) const
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

		void setDirection(sf::Vector2f di)
		{
			// y increases from up to down so we add gravity instead of discarding
			m_vel.y += m_gravity;
			m_vel.x = speed * di.x;
		}

		void update(float dt)
		{
			m_isGettingDamage = false;
			m_isGettingPoints = false;

			if(m_vel.y < 0) // we are jumping
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
		void attack(float dt)
		{
			if (m_vel.x < 0.f)
				currentAnimationType = AnimationType::AttackLeft;
			else // because by default our character looks at right 
				currentAnimationType = AnimationType::AttackRight;

			m_vel.y = 0.f;

			// maybe put this part in some function for avoiding duplication todo
			auto const iAnimation = animationTypeToInt(currentAnimationType);
			m_animations[iAnimation].update(dt);
			m_animations[iAnimation].applyToSprite(m_sprite);
		}

		void jump(float dt)
		{
			// negative because sfml y starts from 0 up and increases down
			if(m_pos.y == m_startPos.y) // to prevent infinite jumping
				m_vel.y = -350.f; // todo this is temp
			update(dt);
			//currentAnimationType = AnimationType::JumpRight;


			//// maybe put this part in some function for avoiding duplication todo
			//auto const iAnimation = animationTypeToInt(currentAnimationType);
			//m_animations[iAnimation].update(dt);
			//m_animations[iAnimation].applyToSprite(m_sprite);
		}

		sf::Rect<float> getCollisionRect()
		{
			sf::RectangleShape rect(sf::Vector2f(m_sprite.getTextureRect().width * m_sprite.getScale().x, m_sprite.getTextureRect().height * m_sprite.getScale().y));
			rect.setPosition(m_sprite.getPosition());

			return rect.getGlobalBounds();
		}

		bool isAttacking()
		{
			return currentAnimationType == AnimationType::AttackLeft || currentAnimationType == AnimationType::AttackRight;
		}

		void getPoints()
		{
			m_isGettingPoints = true;
		}

		void getDamage()
		{
			m_isGettingDamage = true;
		}
	protected:
		CharacterType m_type;
		// todo I think speed should be different from character to another so no static here and passed to ctor
		static constexpr float speed = 150.f;
		sf::Vector2f m_pos;
		sf::Vector2f m_startPos;
		sf::Vector2f m_vel = { 0.0f,0.0f };
		sf::Sprite m_sprite;
		float m_gravity = 1.1f;
		bool m_isGettingDamage = false;
		bool m_isGettingPoints = false;
		// todo add m_
		AnimationType currentAnimationType;
		// Todo support many animations for now just one type (move right)
		std::vector<Animation> m_animations;
	};
}
#endif // !CHARACTER_H