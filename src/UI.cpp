#include <fstream>

#include "UI.hpp"
#include "Character.hpp"
#include "nlohmann/json.hpp"

namespace wonderland {

    using json = nlohmann::json;

    bool UI::loadedHpTexture = false;
    sf::Sprite UI::sprite;
    sf::Texture UI::playerHpTexture;
    sf::Texture UI::reversePlayerHpTexture;
	std::unordered_map<std::string, UI::up<sf::Texture>>  UI::ms_pathToTexture;
    std::vector<ButtonSprite> UI::ms_buttonSprites;

    std::vector<sf::Text> UI::ms_texts;
    std::unordered_map<std::string, UI::up<sf::Font>> UI::ms_pathToFont;


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
        static bool configLoaded = false;
        if (!configLoaded)
        {
            loadConfig();
            configLoaded = true;
        }

        for(auto& s : ms_buttonSprites)
        {
            rt.draw(*s.pSprite);
        }


        // draw the text
        for(auto &t : ms_texts)
        {
            rt.draw(t);
        }

    }

    void UI::loadConfig()
    {
        {
            std::ifstream f(kButtonsConfigPath);
            json data = json::parse(f);

            for (auto a : data)
            {

                const auto& path = a["path"];
                if (!ms_pathToTexture.contains(path))
                {
                    auto tex = std::make_unique<sf::Texture>();
                    tex->loadFromFile(path);
                    ms_pathToTexture[path] = std::move(tex);
                }

                const auto& tex = *ms_pathToTexture[path];

                ButtonSprite bs;
                bs.name = a["name"];
                bs.pSprite = std::make_unique<sf::Sprite>();
                bs.pSprite->setTexture(tex);
                auto texRect = sf::IntRect(a["posInTexture"][0], a["posInTexture"][1], a["size"][0], a["size"][1]);
                bs.pSprite->setTextureRect(texRect);
                bs.pSprite->setScale(a["scale"], a["scale"]);
                bs.pSprite->setPosition(a["posOnScreen"][0], a["posOnScreen"][1]);

                ms_buttonSprites.push_back(std::move(bs));
            }
        }

        {
            std::ifstream ft(kTextConfigPath);
            json data = json::parse(ft);

            for (auto a : data)
            {

                const auto& path = a["fontPath"];
                if (!ms_pathToFont.contains(path))
                {
                    auto font = std::make_unique<sf::Font>();
                    if (!font->loadFromFile(path))
                    {
                        std::cout << "error loading font " << path << "\n";
                        exit(-1);
                    }
                    ms_pathToFont[path] = std::move(font);
                }

                const auto& font = *ms_pathToFont[path];

                sf::Text t;
                t.setFont(font);
                t.setString(std::string(a["string"]));
                t.setCharacterSize(a["size"]); // in pixels, not points!
                if (a["color"] == "Blue")
                    t.setFillColor(sf::Color::Blue);
                else if (a["color"] == "Red")
                    t.setFillColor(sf::Color::Red);
                else // black by default otherwise
                    t.setFillColor(sf::Color::Black);

                // todo add more color cases

                t.setPosition(a["pos"][0], a["pos"][1]);
                ms_texts.push_back(t);
            }
        }

    }
}
