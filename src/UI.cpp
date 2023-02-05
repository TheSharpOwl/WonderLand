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

        for (int i = 0; i < characters.size();i++)
        {
            if(i == playerIdx)
                continue;


            const int yOffset = 22;
            const int xOffset = characters[i]->getCollisionRect().width/6;
            const int ySize = 20;
            const int xSize = 80;
            {
                // the inside hp

                sf::RectangleShape rect(sf::Vector2f(xSize * (characters[i]->getHp() / 100.f), ySize));
                rect.setPosition(characters[i]->getCollisionRect().left + xOffset, characters[i]->getCollisionRect().top - yOffset);
                rect.setOutlineThickness(0.f);
                rect.setFillColor(sf::Color(255.f, 0.f, 0.f));
                rt.draw(rect);
            }

            {
                // the border
                sf::RectangleShape rect(sf::Vector2f(xSize, ySize));
                rect.setPosition(characters[i]->getCollisionRect().left + xOffset, characters[i]->getCollisionRect().top - yOffset);
                rect.setOutlineThickness(3.f);
                rect.setFillColor(sf::Color(0.f, 0.f, 0.f, 1.f));
                rect.setOutlineColor(sf::Color(0, 0, 0)); // black
                rt.draw(rect);
            	}
        }
    }
}