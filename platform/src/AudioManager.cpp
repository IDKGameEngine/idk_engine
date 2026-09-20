#include "idk/platform/AudioManager.hpp"
#include "idk/platform/PlatformContext.hpp"
#include "libidk/log.hpp"
#include <SDL3/SDL.h>


idk::AudioManager::AudioManager(PlatformContext &ctx)
:   IPlatformFeature(ctx)
{
    if (false == MIX_Init())
    {
        VLOG_FATAL("{}", SDL_GetError());
    }

    mMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    IDK_ASSERT(mMixer != NULL, "{}", SDL_GetError());

    for (uint16_t i=0; i<MAX_SOUNDS; i++)
    {
        mFreelist.push(i);
    }
}


idk::AudioManager::~AudioManager()
{
    MIX_Quit();
}


void idk::AudioManager::onUpdate(idk::ServiceManager*)
{

}


idk::AudioManager::SoundType *idk::AudioManager::createSound(const char *filepath)
{
    if (mFreelist.empty())
    {
        VLOG_WARN("[AudioManager::createSound] Cannot create any more sounds!");
        return nullptr;
    }

    uint16_t idx = mFreelist.top(); mFreelist.pop();
    mUsedlist.push(idx);

    SoundType *snd = &mSounds[idx];
    snd->mAudio = MIX_LoadAudio(mMixer, filepath, false);
    IDK_ASSERT(snd->mAudio != NULL, "[AudioManager::loadSound] {}", SDL_GetError());
    snd->mTrack = MIX_CreateTrack(mMixer);
    IDK_ASSERT(snd->mTrack != NULL, "[AudioManager::loadSound] {}", SDL_GetError());
    snd->mIdx = idx;
    MIX_SetTrackAudio(snd->mTrack, snd->mAudio);

    return snd;
}


void idk::AudioManager::destroySound(SoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_DestroyTrack(snd->mTrack);
    MIX_DestroyAudio(snd->mAudio);
    mFreelist.push(snd->mIdx);
    mUsedlist.remove(snd->mIdx);
}


void idk::AudioManager::startSound(SoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_PlayTrack(snd->mTrack, 0);
}


void idk::AudioManager::stopSound(SoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_StopTrack(snd->mTrack, 0);
}


void idk::AudioManager::pauseSound(SoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_PauseTrack(snd->mTrack);
}


void idk::AudioManager::resumeSound(SoundType *isnd)
{
    auto *snd = dynamic_cast<SoundType*>(isnd);
    MIX_ResumeTrack(snd->mTrack);
}

