#ifndef ANIMATION_H
#define ANIMATION_H
// With help of:
// https://github.com/planetchili/sfml_sprite/blob/master/SFMLspriteAnim/Source.cpp
#include <iostream>
namespace wonderland {
	// For now this class will load the animation from many images with names' pattern X1,X2,X3..etc
	// Later might need to support sprite sheets
	class Animation
	{
	public:
		Animation() = default;
		Animation(const std::string& fileName, int nFrames, float holdTime, bool reverseX = false)
			:
			holdTime(holdTime),
			m_reverseX(reverseX)
		{
			// todo check why this is not working
			//frames.reserve(nFrames);
			for (int i = 0; i < nFrames; i++)
			{
				// apply the pattern filename1,filename2..etc
				// todo make extension customizable
				frames.push_back(sf::Texture());
				sf::Image img;
				bool loaded = img.loadFromFile(fileName + std::to_string(i + 1) + ".png");
				if (!loaded)
				{
					std::cout << "error loading the character\n";
					exit(-1);
				}
				if (reverseX)
				{
					img.flipHorizontally();
					frames[i].loadFromImage(img, sf::IntRect(img.getSize().x / 2 + 1, 0, img.getSize().x / 2, img.getSize().y));
				}
				else
					frames[i].loadFromImage(img, sf::IntRect(0, 0, img.getSize().x / 2, img.getSize().y));
			}
		}
		void applyToSprite(sf::Sprite& s) const
		{
			s.setTexture(frames[iFrame]);
		}

		void update(float dt)
		{
			time += dt;
			while (time >= holdTime)
			{
				time -= holdTime;
				advance();
			}
		}
	private:
		void advance()
		{
			// todo use mod
			if (++iFrame >= int(frames.size()))
			{
				iFrame = 0;
			}
		}
	private:
		float holdTime;
		// todo try pointers for textures
		std::vector<sf::Texture> frames;
		int iFrame = 0;
		float time = 0.0f;
		bool m_reverseX;
	};
}
#endif //  ANIMATION_HPP