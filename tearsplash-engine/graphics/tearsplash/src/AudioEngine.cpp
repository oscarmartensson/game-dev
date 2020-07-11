#include <Tearsplash/Audioengine.h>
#include <Tearsplash/Errors.h>

using namespace Tearsplash;

void SoundEffect::play(const int loop) {
    if (Mix_PlayChannel(-1, mChunk, loop) == -1) {
        // If this fails, use channel 0.
        if (Mix_PlayChannel(0, mChunk, loop) == -1) {
            fatalError("Mix_PlayChannel error playing channel -1 " + std::to_string(loop) + " number of times. Mixer error: " + Mix_GetError());
        }
    }
}

void Music::play(const int loop) {
    if (Mix_PlayMusic(mMusic, loop) == -1) {
        fatalError("Mix_PlayMusic error playing channel -1 " + std::to_string(loop) + " number of times. Mixer error: " + Mix_GetError());
    }
}

void Music::pause() {
    Mix_PausedMusic();
}

void Music::stop() {
    Mix_HaltMusic();
}

void Music::resume() {
    Mix_ResumeMusic();
}

AudioEngine::AudioEngine() {
};

AudioEngine::~AudioEngine() {
    destroy();
}

void AudioEngine::init() {
   // Initialize SDL_mixer with a bitwise combination of
    // MIX_INIT_FLAC
    // MIX_INIT_MOD
    // MIX_INIT_MP3
    // MIX_INIT_OGG
    if (Mix_Init(MIX_INIT_MP3) == -1) {
        fatalError("Mix init error " + std::string(Mix_GetError()));
    }

    // Init the Audio to stereo with default frequency.
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        fatalError("Mix init error " + std::string(Mix_GetError()));
    }

    mInitialized = true;
}

void AudioEngine::destroy() {
    if (mInitialized) {
        mInitialized = false;
        Mix_Quit();
    }
}


SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
    // See if filePath sound effect already exists.
    auto it = mEffectMap.find(filePath);

    SoundEffect effect;
    if (it == mEffectMap.end()) {
        // Need to load the effect.
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (!chunk) {
            fatalError("Mix_LoadWAV failed to load effect at " + filePath);
        }
        // Store the effect in the cache map.
        effect.mChunk = chunk;
        mEffectMap[filePath] = chunk;
    }
    else {
        // Effect already cached.
        effect.mChunk = it->second;
    }

    return effect;
}

Music AudioEngine::loadMusic(const std::string& filePath) {
    // See if filePath sound effect already exists.
    auto it = mMusicMap.find(filePath);

    Music music;
    if (it == mMusicMap.end()) {
        // Need to load the effect.
        Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
        if (!mixMusic) {
            fatalError("Mix_LoadMUS failed to load effect at " + filePath);
        }
        // Store the effect in the cache map.
        music.mMusic = mixMusic;
        mMusicMap[filePath] = mixMusic;
    }
    else {
        // Effect already cached.
        music.mMusic = it->second;
    }

    return music;
}