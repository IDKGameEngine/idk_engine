#pragma once

#include "libidk/Service.hpp"
#include "libidk/ObjectManager.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/dsa/Stack.hpp"
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

        ObjectHandle createSound(const char *filepath);
        void destroySound(ObjectHandle);
        void startSound(ObjectHandle);
        void stopSound(ObjectHandle);
        void pauseSound(ObjectHandle);
        void resumeSound(ObjectHandle);

        bool isSoundPlaying(ObjectHandle);
        bool isSoundFinished(ObjectHandle);


    private:
        MIX_Mixer *mMixer;

        idk::ObjectManager<SoundType, MAX_SOUNDS> mSounds;
        // idk::ArrayType<SoundType, MAX_SOUNDS>   mSounds;
        // idk::InplaceStack<uint16_t, MAX_SOUNDS> mFreelist;
        // idk::InplaceStack<uint16_t, MAX_SOUNDS> mUsedlist;

    };

}
