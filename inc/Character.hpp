#ifndef CHARACTER_H
#define CHARACTER_H

#include "Animation.hpp"

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
		Character(sf::Vector2f pos, CharacterType type, std::vector<Animation> animations);

		virtual ~Character() = default;

		void draw(sf::RenderTarget& rt) const;

		void setDirection(sf::Vector2f di);

		virtual void update(float dt);
		// todo maybe apply gravity even while attacking?
		void attack(float dt);

		void jump(float dt);

		// should be called first to reset the data to its initial value before the frame starts (if needed to be reset)
		virtual void reset();

		sf::Rect<float> getCollisionRect() const;

		bool isAttacking() const;

		void getPoints();

		void getDamage();

		int getHp() const;

	protected:
		CharacterType m_type;
		// todo I think speed should be different from character to another so no static here and passed to ctor
		float speed = 150.f;
		int m_hp; // health percentage
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