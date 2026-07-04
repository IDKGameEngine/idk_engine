#pragma once

#include "idk/core/types.hpp"

namespace idk::engine
{
    class LocalGameClient;
    class LocalGameServer;
    class LocalGameService;
}


class idk::engine::LocalGameClient: private idk::NonMobile
{
public:
    friend class LocalGameService;

    LocalGameClient();

private:


};
