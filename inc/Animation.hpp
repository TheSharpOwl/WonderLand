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

		struct AnimationInfo
		{
			std::string fileName; // path of the file of the animation sprite (image) with its name 
			int nFrames; // nFrames number of frames with this animation
			float holdTime; // holdTime how long should a frame last
			sf::Vector2f scale = sf::Vector2f(1.f, 1.f); // scale how much to scale the image we use as a sprite (maybe it will be too small or too big for the background size)
			bool reverseX = false; // reverseX should the animation parts be reversed ? (such as making walking-left animation from a walking-right sprite)
		};


		Animation() = default;
		// todo add option to make the image bigger (because this one used for now is so small so scale factor with 1.f by default)
		/**
		 * \brief 
		 * \param fileName path of the file of the animation sprite (image) with its name 
		 * \param nFrames number of frames with this animation
		 * \param holdTime how long should a frame last
		 * \param scale how much to scale the image we use as a sprite (maybe it will be too small or too big for the background size)
		 * \param reverseX should the animation parts be reversed ? (such as making walking-left animation from a walking-right sprite)
		 */
		Animation(const std::string& fileName, int nFrames, float m_holdTime, sf::Vector2f scale = sf::Vector2f(1.f, 1.f), bool reverseX = false)
			:
			m_holdTime(m_holdTime),
			m_reverseX(reverseX),
			m_scale(std::move(scale))
		{
			m_frames.resize(nFrames);

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
					m_frames[nFrames - i - 1].loadFromImage(img, sf::IntRect(i * img.getSize().x / nFrames, 0, img.getSize().x / nFrames, img.getSize().y));
				}
				else
					m_frames[i].loadFromImage(img, sf::IntRect(i * img.getSize().x/nFrames, 0, img.getSize().x / nFrames, img.getSize().y));
			}
		}

		void applyToSprite(sf::Sprite& s) const
		{
			s.setTexture(m_frames[m_iFrame]);
			s.setScale(m_scale.x, m_scale.y);
		}

		void update(float dt)
		{
			m_time += dt;
			while (m_time >= m_holdTime)
			{
				m_time -= m_holdTime;
				advance();
			}
		}
	private:
		void advance()
		{
			// todo use mod
			if (++m_iFrame >= int(m_frames.size()))
			{
				m_iFrame = 0;
			}
		}
	private:
		float m_holdTime;
		// todo try pointers for textures
		std::vector<sf::Texture> m_frames;
		// todo find a better solution than doing this every frame
		// scale for each frame (when applying the sprite)
		sf::Vector2f m_scale;
		int m_iFrame = 0;
		float m_time = 0.0f;
		bool m_reverseX;
	};
}
#endif //  ANIMATION_HPP