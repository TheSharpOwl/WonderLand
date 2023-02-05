#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"

namespace wonderland {

    class Character;
    class UI {
    public:
        // Todo make UI a singelton class maybe?
        static void draw(sf::RenderTarget &rt, std::vector<std::shared_ptr<Character>>, int playerIdx);

        static sf::Sprite sprite;
        static sf::Texture playerHpTexture;
        static sf::Texture reversePlayerHpTexture;
        static bool loadedHpTexture;
    };
}
