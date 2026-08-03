
#include "libidk/platform/PlatformContext.hpp"
#include "libidk/platform/SDL3FileSystem.hpp"
#include "libidk/platform/SDL3Time.hpp"
#include "libidk/platform/SDL3Events.hpp"
#include "libidk/platform/SDL3Video.hpp"



int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::srand(clock());

    using namespace idk;

    PlatformContext ctx = PlatformContext::BuildPlatformContext
        <SDL3FileSystem, SDL3Time, SDL3Events, SDL3Video>();

    static idk::Platform plat;
    static idk::GfxService gfxSrv(plat);
    static idk::GameService gameSrv;
    static idk::Engine engine(plat, {&gfxSrv, &gameSrv});

    while (engine.running())
    {
        engine.update();
    }

    return 0;
}
