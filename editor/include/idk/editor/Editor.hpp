#pragma once

#include "libidk/Service.hpp"


namespace idk::editor
{
    class EditorApplication: public idk::Service
    {
    public:
        virtual void onInit(idk::EngineAPI&) final;
        virtual void onShutdown(idk::EngineAPI&) final;
        virtual void onPreRender(idk::EngineAPI&) final;
        virtual void onMidRender(idk::EngineAPI&) final;

    private:
        static void onGenericEventCallback(idk::EngineAPI&, const void*);

    };
}

