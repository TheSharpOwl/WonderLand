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
		Character(sf::Vector2f pos) : m_pos(std::move(pos))
		{
			// todo last parameter is random
			m_animation = Animation("../assets/Mage/Walk/walk", 6,0.1f );
		}

		void draw(sf::RenderTarget& rt) const
		{
			rt.draw(m_sprite);
		}

		void setDirection(sf::Vector2f di = sf::Vector2f(1.f, 0.f))
		{
			m_vel = speed * di;
		}
		void update(float dt)
		{
			m_pos += m_vel * dt;
			m_animation.update(dt);
			m_animation.applyToSprite(m_sprite);
		}

	private:
		// todo I think speed should be different from character to another so no static here and passed to ctor
		static constexpr float speed = 100.0f;
		sf::Vector2f m_pos;
		sf::Vector2f m_vel = { 0.0f,0.0f };
		sf::Sprite m_sprite;
		// Todo support many animations for now just one type (move right)
		Animation m_animation;
	};
}
#endif // !CHARACTER_H