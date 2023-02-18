#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include "SFML/Graphics.hpp"

namespace wonderland {

    class Character;


    struct ButtonSprite
    {
        std::string name;
        std::unique_ptr<sf::Sprite> pSprite;
    };

    class UI {
    public:

        template<typename T>
        using up = std::unique_ptr<T>;



        // Todo make UI a singelton class maybe?
        static void draw(sf::RenderTarget &rt, std::vector<std::shared_ptr<Character>>, int playerIdx);

        static void showControls(sf::RenderTarget& rt);
        static sf::Sprite sprite;
        static sf::Texture playerHpTexture;
        static sf::Texture reversePlayerHpTexture;
        static bool loadedHpTexture;

        static void loadConfig();

    private:
        // to load an image only once (so we got only one texture for each unique image)
        static std::unordered_map<std::string, up<sf::Texture>> ms_pathToTexture;
        static std::vector<ButtonSprite> ms_buttonSprites;

        static inline const std::string kButtonsConfigPath = "../config/ButtonsTextures.json";
    };
}
