#pragma once

#include "libidk/Service.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/dsa/Stack.hpp"
#include "libidk/dsa/ResourceManager.hpp"
#include <SDL3_mixer/SDL_mixer.h>


namespace idk
{
    class AudioManager: public idk::Service
    {
    public:
        static constexpr uint16_t MAX_SOUNDS = 64;

        class SoundType
        {
        private:
            friend class idk::AudioManager;
            MIX_Audio *mAudio;
            MIX_Track *mTrack;
            bool       mStarted;
            bool       mFinished;

            SoundType(): SoundType(nullptr, nullptr) {  };
            SoundType(MIX_Audio *a, MIX_Track *t): mAudio(a), mTrack(t), mStarted(false), mFinished(false) {  };
        };

        virtual void onInit(EngineAPI&) final;
        virtual void onShutdown(EngineAPI&) final;
        virtual void onPreFrame(EngineAPI&) final;

        ResourceHandle createSound(const char *filepath);
        void destroySound(ResourceHandle);
        void startSound(ResourceHandle);
        void stopSound(ResourceHandle);
        void pauseSound(ResourceHandle);
        void resumeSound(ResourceHandle);

        bool isSoundPlaying(ResourceHandle);
        bool isSoundFinished(ResourceHandle);


    private:
        MIX_Mixer *mMixer;

        idk::ResourceManager<SoundType, MAX_SOUNDS> mSounds;
        // idk::ArrayType<SoundType, MAX_SOUNDS>   mSounds;
        // idk::InplaceStack<uint16_t, MAX_SOUNDS> mFreelist;
        // idk::InplaceStack<uint16_t, MAX_SOUNDS> mUsedlist;

    };

}
