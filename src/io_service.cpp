#include "idk/io_service.hpp"
#include "idk/core/log.hpp"

#include <SDL3/SDL.h>


idk::IoService::IoService()
:   Service(1000.0 / 60.0),
    cmd_read_(cmd_queue_),
    alive(true),
    flush(false)
{

}


void idk::IoService::_startup(idk::IEngine*)
{
    VLOG_INFO("[idk::IoService::_startup]");
    // idk::FileLoader f1("asset/")
}


void idk::IoService::_update(idk::IEngine*)
{
    while (!cmd_read_->empty())
    {
        auto &req = cmd_read_->front();
        auto *res = req.res;

        switch (req.type)
        {
            case IoReqType::LoadFile:
                break;

            case IoReqType::WriteFile:
                break;

            default:
                VLOG_FATAL("Invalid IoReqType");
                break;
        }

        res->make_ready();
        cmd_read_->pop();
    }

    if (flush.load() == true)
    {
        cmd_queue_.swapBuffers();
        flush.store(false);
    }
}


void idk::IoService::_shutdown(idk::IEngine*)
{
    VLOG_INFO("[idk::IoService::_shutdown]");
}

