#include "SDLAudioPlayer.h"
#include <spdlog/spdlog.h>

namespace Ship {

SDLAudioPlayer::~SDLAudioPlayer() {
    SPDLOG_TRACE("destruct SDL audio player");
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool SDLAudioPlayer::DoInit() {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SPDLOG_ERROR("SDL init error: %s\n", SDL_GetError());
        return false;
    }
    const SDL_AudioSpec spec = { SDL_AUDIO_S16, 2, this->GetSampleRate() };
    mAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
    if (mAudioStream == nullptr) {
        SPDLOG_ERROR("SDL_OpenAudio error: {}", SDL_GetError());
        return false;
    }
    SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(mAudioStream));
    return true;
}

int SDLAudioPlayer::Buffered() {
    // we were using https://wiki.libsdl.org/SDL2/SDL_GetQueuedAudioSize which according to
    // the migration docs (https://wiki.libsdl.org/SDL3/README/migration#sdl_audioh)
    //     SDL_QueueAudio(), SDL_DequeueAudio, and SDL_ClearQueuedAudio and
    //     SDL_GetQueuedAudioSize() have been removed; an SDL_AudioStream bound
    //     to a device provides the exact same functionality.
    // it's not clear what exactly we should be using instead, i found
    // https://wiki.libsdl.org/SDL3/SDL_GetAudioStreamAvailable
    // https://wiki.libsdl.org/SDL3/SDL_GetAudioStreamQueued
    // the SDL2 one (SDL_GetQueuedAudioSize) says
    //     Get the number of bytes of still-queued audio.
    // SDL_GetAudioStreamAvailable says
    //     Get the number of converted/resampled bytes available.
    // SDL_GetAudioStreamQueued says
    //     Get the number of bytes currently queued.
    // i'm trying SDL_GetAudioStreamQueued for now

    // 4 is sizeof(int16_t) * num_channels (2 for stereo)
    return SDL_GetAudioStreamQueued(mAudioStream) / 4;
}

void SDLAudioPlayer::Play(const uint8_t* buf, size_t len) {
    if (Buffered() < 6000) {
        // Don't fill the audio buffer too much in case this happens
        SDL_PutAudioStreamData(mAudioStream, buf, len);
    }
}
} // namespace Ship
