#include "UI.hpp"
#include "Character.hpp"

namespace wonderland {
    bool UI::loadedHpTexture = false;
    sf::Sprite UI::sprite;
    sf::Texture UI::playerHpTexture;

    void UI::draw(sf::RenderTarget &rt, std::vector<std::shared_ptr<Character>>, int playerIdx) {
        if(!loadedHpTexture)
        {
            playerHpTexture.loadFromFile("../assets/health.png");
            loadedHpTexture = true;
        }

        // todo change it according to percentage
        sprite.setTexture(playerHpTexture);
        sprite.setPosition(sf::Vector2f(10.f, 10.f));
        sprite.setScale(0.5, 0.5);
        rt.draw(sprite);
    }
}