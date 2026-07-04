#pragma once

#include "idk/core/types.hpp"

namespace idk::engine
{
    class LocalGameClient;
}


class idk::engine::LocalGameClient: private idk::NonMobile
{
public:
    LocalGameClient();

};
