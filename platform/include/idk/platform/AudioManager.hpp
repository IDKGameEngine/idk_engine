#pragma once

#include "IPlatformFeature.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/dsa/Stack.hpp"
#include <SDL3_mixer/SDL_mixer.h>


namespace idk
{
    class AudioManager: public idk::IPlatformFeature
    {
    public:
        static constexpr uint16_t MAX_SOUNDS = 64;

        class SoundType
        {
        private:
            friend class idk::AudioManager;
            MIX_Audio *mAudio;
            MIX_Track *mTrack;
            uint16_t   mIdx;
        };

        AudioManager();
        ~AudioManager();

        virtual void onUpdate(idk::PlatformContext&) final;
        virtual SoundType *createSound(const char *filepath);
        virtual void destroySound(SoundType*) final;
        virtual void startSound(SoundType*) final;
        virtual void stopSound(SoundType*) final;
        virtual void pauseSound(SoundType*) final;
        virtual void resumeSound(SoundType*) final;

    private:
        MIX_Mixer *mMixer;
    
        idk::ArrayType<SoundType, MAX_SOUNDS>   mSounds;
        idk::InplaceStack<uint16_t, MAX_SOUNDS> mFreelist;
        idk::InplaceStack<uint16_t, MAX_SOUNDS> mUsedlist;

    };

}
