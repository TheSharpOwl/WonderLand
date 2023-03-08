#ifndef AUDIO_H
#define AUDIO_H
#include <irrKlang.h>
#include <string>

namespace wonderland
{
    class Audio
    {
    protected:
        Audio() = default;

        ~Audio();

        // todo use unique_ptr and remove dctor
        static irrklang::ISoundEngine* ms_engine;

    public:

        Audio(Audio& other) = delete;
        void operator= (const Audio& other) = delete;


        // seems no need to get the instance from outside so no method for that for now

        static void createEngine();


        static void startBackgroundMusic(const std::string& backgroundMusicPath);

    };

}
#endif