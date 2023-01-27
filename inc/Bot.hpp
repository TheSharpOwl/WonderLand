#ifndef BOT_H
#define BOT_H

#include "Character.hpp"

namespace wonderland
{
    /**
     * @brief For now Bot class is only for enemies so a bot is an enemy always
     */
    class Bot : public Character
    {
    public:
        using Base = Character;

        Bot(sf::Vector2f pos, std::vector<Animation> animations);

        virtual void update(float dt) override;

        void updatePlayerRect(sf::Rect<float> playerRect);
    protected:

        inline static float kSecondsBetweenAttacks = 1.2f;

        float m_lastAttackTime = 0.f;
        sf::Rect<float> m_playerRect;
    };
}
#endif // !BOT_H
