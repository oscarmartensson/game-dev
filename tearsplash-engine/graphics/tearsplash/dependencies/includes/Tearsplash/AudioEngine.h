#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <string>

#include <SDL/SDL_mixer.h>
#include <map>

namespace Tearsplash {

    class SoundEffect {
    public:
        friend class AudioEngine;

        // Plays a sound effect on the default -1 channel.
        // 0 plays the sound effect 1 time,
        // -1 plays the sound effect infinite amount of times,
        // n plays the sound effect n amounts of time.
        void play(const int loop = 0);

    private:
        Mix_Chunk* mChunk;
    };

    class Music {
    public:
        friend class AudioEngine;

        // 1 plays the music infinite 1 time,
        // n plays the sound effect n amounts of time.
        void play(const int loop = 1);

        // Pauses the currently playing music.
        static void pause();

        // Stops the currently playing music.
        static void stop();

        // Resumse the currently playing music.
        static void resume();

    private:
        Mix_Music* mMusic;
    };

    class AudioEngine
    {
    public:
        AudioEngine();
        ~AudioEngine();

        void init();
        void destroy();

        SoundEffect loadSoundEffect(const std::string& filePath);
        Music loadMusic(const std::string& filePath);

    private:
        bool mInitialized = false;
        std::map<std::string, Mix_Chunk*> mEffectMap;
        std::map<std::string, Mix_Music*> mMusicMap;
    };
}

#endif // AUDIOENGINE_H
