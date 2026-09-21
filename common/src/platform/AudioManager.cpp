#include "idk/platform/AudioManager.hpp"
#include "idk/EngineAPI.hpp"

#include <SDL3/SDL.h>


void idk::AudioManager::onInit(EngineAPI&)
{
    if (false == MIX_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    mMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    IDK_ASSERT(mMixer != NULL, "{}", SDL_GetError());
}

void idk::AudioManager::onShutdown(EngineAPI&)
{
    MIX_Quit();
}


void idk::AudioManager::onPreFrame(EngineAPI&)
{
    // for (uint16_t i=0; i<MAX_SOUNDS; i++)
    // {
    //     auto &snd = mSounds[i];
    //     if (!(snd.mAudio && snd.mTrack))
    //     {
    //         continue;
    //     }
    //     if (snd.mStarted && !MIX_TrackPlaying(snd.mTrack))
    //     {
    //         snd.mFinished = true;
    //     }
    // }
}


idk::ResourceHandle idk::AudioManager::createSound(const char *filepath)
{
    MIX_Audio *audio = MIX_LoadAudio(mMixer, filepath, false);
    IDK_ASSERT(audio != NULL, "[AudioManager::loadSound] {}", SDL_GetError());

    MIX_Track *track = MIX_CreateTrack(mMixer);
    IDK_ASSERT(track != NULL, "[AudioManager::loadSound] {}", SDL_GetError());

    SoundType snd(audio, track);
    MIX_SetTrackAudio(snd.mTrack, snd.mAudio);

    return mSounds.createResource(snd);
}


void idk::AudioManager::destroySound(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_DestroyTrack(snd->mTrack);
    MIX_DestroyAudio(snd->mAudio);
    mSounds.destroyResource(H);
}


void idk::AudioManager::startSound(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    snd->mStarted = true;
    MIX_PlayTrack(snd->mTrack, 0);
}


void idk::AudioManager::stopSound(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_StopTrack(snd->mTrack, 0);
}


void idk::AudioManager::pauseSound(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_PauseTrack(snd->mTrack);
}


void idk::AudioManager::resumeSound(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_ResumeTrack(snd->mTrack);
}


bool idk::AudioManager::isSoundPlaying(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    return MIX_TrackPlaying(snd->mTrack);
}


bool idk::AudioManager::isSoundFinished(ResourceHandle H)
{
    auto *snd = mSounds.get(H);
    return snd->mFinished;
}


