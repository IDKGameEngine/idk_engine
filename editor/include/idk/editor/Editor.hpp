#pragma once

#include "idk/engine/IApplication.hpp"


namespace idk::editor
{
    class EditorApplication: public idk::IApplication
    {
    public:
        EditorApplication();
        virtual void onInit(idk::Engine&) final;
        virtual void onUpdate(idk::Engine&) final;
        virtual void onShutdown(idk::Engine&) final;

    private:

    };
}
