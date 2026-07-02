#include "idk/engine.hpp"
#include "idk/core/log.hpp"

int main(int argc, char **argv)
{
    VLOG_INFO("[main]");
    srand(clock());

    if (!idk::platform::AppInit())
    {
        return 1;
    }

    return idk::platform::AppEntry(argc, argv);
}
