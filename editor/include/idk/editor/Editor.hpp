#pragma once

#include "idk/engine/IApplication.hpp"


namespace idk::editor
{
    class EditorApplication: public idk::IApplication
    {
    public:
        virtual void onInit(idk::EngineAPI&) final;
        virtual void onShutdown(idk::EngineAPI&) final;
        virtual void onPreFrame(idk::EngineAPI&) final;
        virtual void onMidFrame(idk::EngineAPI&) final;
        virtual void onPostFrame(idk::EngineAPI&) final;
        virtual void onEvent(idk::EngineAPI&, const void*) final;

    private:

    };
}

