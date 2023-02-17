#include "Audio.hpp"

namespace wonderland
{
	irrklang::ISoundEngine* Audio::ms_engine = nullptr;



	Audio::~Audio()
	{
		if (ms_engine)
		{
			ms_engine->drop();
		}
	}

	void Audio::createEngine()
	{
		if (!ms_engine)
		{
			ms_engine = irrklang::createIrrKlangDevice();
		}
	}

	void Audio::startBackgroundMusic()
	{
		createEngine();
		irrklang::ISound* snd = ms_engine->play2D(kBackgroundMusicPath.c_str(), true, false, true);
		if (snd)
		{
			// todo add volume controller and change sound from keyboard
			snd->setVolume(0.01f);
		}

	}

};
