#ifndef CHARACTER_H
#define CHARACTER_H

#include "Animation.hpp"

#include <algorithm>

// for debugging strings
#include <Windows.h>

/*
 * with help of:
 * https://github.com/planetchili/sfml_sprite/blob/master/SFMLspriteAnim/Source.cpp
 */

namespace wonderland
{
	class Character
	{
	public:
		// todo support adding a character using json file (need nlohman::json for that)
		Character(sf::Vector2f pos, std::vector<Animation> animations) : m_startPos(std::move(pos)), m_animations(std::move(animations))
		{
			m_pos = m_startPos;
		}

		virtual ~Character() = default;

		void draw(sf::RenderTarget& rt) const
		{
			rt.draw(m_sprite);
		}

		void setDirection(sf::Vector2f di)
		{
			// y increases from up to down so we add gravity instead of discarding
			m_vel.y += m_gravity;
			m_vel.x = speed * di.x;
		}

		void update(float dt)
		{
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
	private:
		// todo I think speed should be different from character to another so no static here and passed to ctor
		static constexpr float speed = 150.f;
		sf::Vector2f m_pos;
		sf::Vector2f m_startPos;
		sf::Vector2f m_vel = { 0.0f,0.0f };
		sf::Sprite m_sprite;
		float m_gravity = 1.1f;
		AnimationType currentAnimationType;
		// Todo support many animations for now just one type (move right)
		std::vector<Animation> m_animations;
	};
}
#endif // !CHARACTER_H