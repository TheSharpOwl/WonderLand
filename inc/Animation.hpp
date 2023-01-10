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
		// todo add option to make the image bigger (because this one used for now is so small so scale factor with 1.f by default)
		Animation(const std::string& fileName, int nFrames, float holdTime, sf::Vector2f scale = sf::Vector2f(1.f, 1.f), bool reverseX = false)
			:
			holdTime(holdTime),
			m_reverseX(reverseX),
			m_scale(std::move(scale))
		{
			frames.resize(nFrames);

			sf::Image img;
			bool loaded = img.loadFromFile(fileName);
			if (!loaded)
			{
				std::cout << "error loading the animation file " << fileName << "\n";
				exit(-1);
			}


			if (reverseX)
				img.flipHorizontally();

			for (int i = 0; i < nFrames; i++)
			{

				if (reverseX)
				{
					// start from the back (first image is the last one and after it is before last ..etc)
					frames[nFrames - i - 1].loadFromImage(img, sf::IntRect(i * img.getSize().x / nFrames, 0, img.getSize().x / nFrames, img.getSize().y));
				}
				else
					frames[i].loadFromImage(img, sf::IntRect(i * img.getSize().x/nFrames, 0, img.getSize().x / nFrames, img.getSize().y));
			}
		}

		void applyToSprite(sf::Sprite& s) const
		{
			s.setTexture(frames[iFrame]);
			s.setScale(m_scale.x, m_scale.y);
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
		// todo find a better solution than doing this every frame
		// scale for each frame (when applying the sprite)
		sf::Vector2f m_scale;
		// todo rename priate members to start with m_
		int iFrame = 0;
		float time = 0.0f;
		bool m_reverseX;
	};
}
#endif //  ANIMATION_HPP