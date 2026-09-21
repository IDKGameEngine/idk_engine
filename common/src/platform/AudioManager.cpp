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


void idk::AudioManager::onUpdate(EngineAPI&)
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


void idk::AudioManager::onEvent(EngineAPI&, const void*)
{

}


idk::ObjectHandle idk::AudioManager::createSound(const char *filepath)
{
    MIX_Audio *audio = MIX_LoadAudio(mMixer, filepath, false);
    IDK_ASSERT(audio != NULL, "[AudioManager::loadSound] {}", SDL_GetError());

    MIX_Track *track = MIX_CreateTrack(mMixer);
    IDK_ASSERT(track != NULL, "[AudioManager::loadSound] {}", SDL_GetError());

    SoundType snd(audio, track);
    MIX_SetTrackAudio(snd.mTrack, snd.mAudio);

    return mSounds.createObject(snd);
}


void idk::AudioManager::destroySound(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_DestroyTrack(snd->mTrack);
    MIX_DestroyAudio(snd->mAudio);
}


void idk::AudioManager::startSound(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    snd->mStarted = true;
    MIX_PlayTrack(snd->mTrack, 0);
}


void idk::AudioManager::stopSound(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_StopTrack(snd->mTrack, 0);
}


void idk::AudioManager::pauseSound(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_PauseTrack(snd->mTrack);
}


void idk::AudioManager::resumeSound(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    MIX_ResumeTrack(snd->mTrack);
}


bool idk::AudioManager::isSoundPlaying(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    return MIX_TrackPlaying(snd->mTrack);
}


bool idk::AudioManager::isSoundFinished(ObjectHandle H)
{
    auto *snd = mSounds.get(H);
    return snd->mFinished;
}


