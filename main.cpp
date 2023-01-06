#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	// load the forest background
	sf::Texture backgroundTexture;
	// TODO set path to something that works in all computers in a better way
	if (!backgroundTexture.loadFromFile("../assets/game_background_1.png"))
	{
		std::cout << "error loading texture";
		exit(-1);
	}

	const auto width = 1280.f;
	const auto height = 720.f;

	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(width/backgroundTexture.getSize().x , height/backgroundTexture.getSize().y );

	//const sf::Vector2f backgroundSize(backgroundSprite.getTexture()->getSize().x * backgroundSprite.getScale().x,
	//	backgroundSprite.getTexture()->getSize().y * backgroundSprite.getScale().y);



	sf::RenderWindow window(sf::VideoMode(width, height), "Wonderland");
	

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.draw(backgroundSprite);
		window.display();
	}

	return 0;
}