#ifndef AUDIO_H
#define AUDIO_H
#include <windows.h>
#pragma comment(lib, "winmm.lib")

namespace wonderland
{
    class Audio
    {
    public:
        static void startBackgroundMusic()
        {
            PlaySound("../assets/xDeviruchi - Decisive Battle.wav", NULL, SND_LOOP | SND_ASYNC);
        }

    private:
    };
}
#endif