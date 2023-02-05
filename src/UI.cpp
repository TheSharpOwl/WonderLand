#include "UI.hpp"
#include "Character.hpp"

namespace wonderland {
    bool UI::loadedHpTexture = false;
    sf::Sprite UI::sprite;
    sf::Texture UI::playerHpTexture;
    sf::Texture UI::reversePlayerHpTexture;

    void UI::draw(sf::RenderTarget &rt, std::vector<std::shared_ptr<Character>> characters, int playerIdx) {
        if(!loadedHpTexture)
        {
            playerHpTexture.loadFromFile("../assets/health.png");
            reversePlayerHpTexture.loadFromFile("../assets/healthInverse.png");
            loadedHpTexture = true;
        }

        // todo change it according to percentage
        sprite.setTexture(playerHpTexture);
        sprite.setPosition(sf::Vector2f(10.f, 10.f));
        sprite.setScale(0.5, 0.5);
        sprite.setTextureRect(sf::IntRect(0, 0, playerHpTexture.getSize().x, playerHpTexture.getSize().y));
        rt.draw(sprite);

        int goldNumber = 160;
        int totalHpWidth = reversePlayerHpTexture.getSize().x - goldNumber;
        float temp = goldNumber + characters[playerIdx]->getHp()/100.f * static_cast<float>(totalHpWidth);
        sprite.setTexture(reversePlayerHpTexture);
        sprite.setPosition(sf::Vector2f(10.f + temp/2, 10.f));
        sprite.setScale(0.5, 0.5);
        sprite.setTextureRect(sf::Rect<int>(temp, 0, reversePlayerHpTexture.getSize().x - temp, reversePlayerHpTexture.getSize().y));

        rt.draw(sprite);
    }
}