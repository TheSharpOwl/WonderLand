#ifndef ANIMATION_H
#define ANIMATION_H
// With help of:
// https://github.com/planetchili/sfml_sprite/blob/master/SFMLspriteAnim/Source.cpp
#include <iostream>

#include "SFML/Graphics.hpp"

namespace wonderland {
	// For now this class will load the animation from many images with names' pattern X1,X2,X3..etc
	// Later might need to support sprite sheets


			// todo add later more animations
	enum class AnimationType
	{
		WalkingRight,
		WalkingLeft,
		Idle,
		AttackRight,
		AttackLeft,
		JumpRight,
		JumpLeft,
		Count
	};

	static int animationTypeToInt(AnimationType type)
	{
		return static_cast<int>(type);
	}

	// todo I think this struct is not needed and the one after it but let's just keep it for now until using config file for a character (json for example)
	struct AnimationInfo
	{
		std::string fileName; // path of the file of the animation sprite (image) with its name 
		int nFrames; // nFrames number of frames with this animation
		float holdTime; // holdTime how long should a frame last
		sf::Vector2f scale = sf::Vector2f(1.f, 1.f); // scale how much to scale the image we use as a sprite (maybe it will be too small or too big for the background size)
		bool reverseX = false; // reverseX should the animation parts be reversed ? (such as making walking-left animation from a walking-right sprite)
	};

	struct AnimationTypeWithInfo
	{
		AnimationType type;
		AnimationInfo info;

		AnimationTypeWithInfo(AnimationType t, AnimationInfo i) : type(t), info(i) {}
	};

	struct TextureChangeInfo
	{
		sf::Vector2f scale = { 1.f, 1.f };
		float upCut = 0.f;
		float downCut = 0.f;
		float rightCut = 0.f;
		float leftCut = 0.f;
	};

	class Animation
	{
	public:
		// todo add animation ctor taking AnimationInfo, make std::vector inside character class as a parameter in the ctor of a character and initialize animations from there
		Animation() = default;
		virtual ~Animation() = default;
		// todo add option to make the image bigger (because this one used for now is so small so scale factor with 1.f by default)
		/**
		 * \brief 
		 * \param fileName path of the file of the animation sprite (image) with its name 
		 * \param nFrames number of frames with this animation
		 * \param holdTime how long should a frame last
		 * \param scale how much to scale the image we use as a sprite (maybe it will be too small or too big for the background size)
		 * \param reverseX should the animation parts be reversed ? (such as making walking-left animation from a walking-right sprite)
		 */
		Animation(const std::string& fileName, int nFrames, float m_holdTime, TextureChangeInfo textureChangeInfo = {}, bool reverseX = false)
			:
			m_holdTime(m_holdTime),
			m_reverseX(reverseX),
			m_scale(textureChangeInfo.scale)
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
					m_frames[nFrames - i - 1].loadFromImage(img, sf::IntRect(i * img.getSize().x / nFrames + textureChangeInfo.leftCut, textureChangeInfo.upCut, img.getSize().x / nFrames - textureChangeInfo.leftCut - textureChangeInfo.rightCut, img.getSize().y - textureChangeInfo.upCut - textureChangeInfo.downCut));
				}
				else
					m_frames[i].loadFromImage(img, sf::IntRect(i* img.getSize().x / nFrames + textureChangeInfo.leftCut, textureChangeInfo.upCut, img.getSize().x / nFrames - textureChangeInfo.leftCut - textureChangeInfo.rightCut, img.getSize().y - textureChangeInfo.upCut - textureChangeInfo.downCut));
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