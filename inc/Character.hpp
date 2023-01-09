#ifndef CHARACTER_H
#define CHARACTER_H

#include "Animation.hpp"

/*
 * with help of:
 * https://github.com/planetchili/sfml_sprite/blob/master/SFMLspriteAnim/Source.cpp
 */

namespace wonderland
{
	class Character
	{
	public:

		// todo add later more animations
		enum class AnimationType
		{
			WalkingRight,
			WalkingLeft,
			Idle,
			Count
		};

		static int animationTypeToInt(AnimationType type)
		{
			return static_cast<int>(type);
		}

		Character(sf::Vector2f pos) : m_pos(std::move(pos))
		{
			m_animations.resize(animationTypeToInt(AnimationType::Count));
			// todo last parameter is random
			m_animations[animationTypeToInt(AnimationType::WalkingRight)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f);
			m_animations[animationTypeToInt(AnimationType::WalkingLeft)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Walk_6.png", 6, 0.1f, true);
			m_animations[animationTypeToInt(AnimationType::Idle)] = Animation("../assets/2_Owlet_Monster/Owlet_Monster_Idle_4.png", 4, 0.1f);
		}

		void draw(sf::RenderTarget& rt) const
		{
			rt.draw(m_sprite);
		}

		void setDirection(sf::Vector2f di)
		{
			m_vel = speed * di;
		}
		void update(float dt)
		{
			if (m_vel.x > 0.f)
				currentAnimationType = AnimationType::WalkingRight;
			else if (m_vel.x < 0.f)
				currentAnimationType = AnimationType::WalkingLeft;
			else
				currentAnimationType = AnimationType::Idle;

			m_pos += m_vel * dt;
			auto const iAnimation = animationTypeToInt(currentAnimationType);
			m_animations[iAnimation].update(dt);
			m_animations[iAnimation].applyToSprite(m_sprite);
			m_sprite.setPosition(m_pos);
		}

	private:
		// todo I think speed should be different from character to another so no static here and passed to ctor
		static constexpr float speed = 100.0f;
		sf::Vector2f m_pos;
		sf::Vector2f m_vel = { 0.0f,0.0f };
		sf::Sprite m_sprite;
		AnimationType currentAnimationType;
		// Todo support many animations for now just one type (move right)
		std::vector<Animation> m_animations;
	};
}
#endif // !CHARACTER_H