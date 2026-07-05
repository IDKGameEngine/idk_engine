#pragma once

#include "idk/core/Engine.hpp"
#include "idk/core/Event.hpp"

namespace idk::engine
{
    class GameWorld;
}


class idk::engine::GameWorld: public idk::NonMobile
{
public:
    GameWorld();
    ~GameWorld();
    void update();

private:
    idk::EventChannel<float> mEventChannel;

};

