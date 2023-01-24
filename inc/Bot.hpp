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
        Bot(sf::Vector2f pos, std::vector<Animation> animations) : Character(pos, CharacterType::Enemy, std::move(animations))
        {
        }
    };
}
#endif // !BOT_HE#define BOT_H
