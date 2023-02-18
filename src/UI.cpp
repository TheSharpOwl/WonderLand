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

    void UI::showControls(sf::RenderTarget& rt)
    {
        // load the texture
        sf::Texture smallButtonsTexture;
        smallButtonsTexture.loadFromFile("../assets/Keyboard Letters and Symbols.png");
        std::vector<std::unique_ptr<sf::Sprite>> tSprite;
        // 8 * 14
        // 128 * 244
        // todo add lambda to do calculation based on position
        std::vector<std::pair<int, int>> buttonPositions = { {0,0}, {128 / 8, 0}, {128 / 8 * 2, 0}, {128 / 8 * 3, 0}, {128/8 * 5, 224/14 * 2} };



        tSprite.resize(buttonPositions.size());

        for(int i =  0 ; i < buttonPositions.size();i ++)
        {
            tSprite[i] = std::make_unique<sf::Sprite>();
            tSprite[i]->setTexture(smallButtonsTexture);
            tSprite[i]->setTextureRect(sf::IntRect(buttonPositions[i].first, buttonPositions[i].second, 128 / 8, 224 / 14));
            tSprite[i]->setScale(5.f, 5.f);
            tSprite[i]->setPosition(sf::Vector2f((i % 4) * 200, (i/4 + 1) * 200));
            rt.draw(*tSprite[i]);
        }



        sf::Texture BigButtonsTexture;
        BigButtonsTexture.loadFromFile("../assets/Keyboard Extras.png");
        // 8 * 4
        std::vector < std::pair<int, int>> BigButtonPositions = { {2 * 128 / 4, 2 * 128 / 8}, {1 * 128 / 4, 0} };
        std::vector<std::unique_ptr<sf::Sprite>> tSprite2;
        tSprite2.resize(BigButtonPositions.size());
        for(int i = 0; i < BigButtonPositions.size();i++)
        {
            tSprite2[i] = std::make_unique<sf::Sprite>();
            tSprite2[i]->setTexture(BigButtonsTexture);
            tSprite2[i]->setTextureRect(sf::IntRect(BigButtonPositions[i].first, BigButtonPositions[i].second, 128 / 4, 128 / 8));
            tSprite2[i]->setScale(5.f, 5.f);
            tSprite2[i]->setPosition(sf::Vector2f((i % 4) * 200, (i / 4 + 1) * 200));
            rt.draw(*tSprite2[i]);
        }

        // todo next set position 


    }
}
