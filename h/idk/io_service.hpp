#pragma once

#include "idk/core/double_buffer.hpp"
#include "idk/core/service.hpp"
#include "idk/core/file.hpp"

#include "idk/engine/io_interface.hpp"


namespace idk
{
    class IoService: public idk::core::Service
    {
    private:
        DoubleBuffer<IoRequest> cmd_queue_;
        DblBufferReader<IoRequest> cmd_read_;

    public:
        std::atomic_bool alive;
        std::atomic_bool flush;

        IoService();
        virtual void _startup(idk::IEngine*) final;
        virtual void _update(idk::IEngine*) final;
        virtual void _shutdown(idk::IEngine*) final;

        idk::DblBufferWriter<IoRequest> getRequestWriter()
        {
            return idk::DblBufferWriter<IoRequest>(cmd_queue_);
        }
    };
}

