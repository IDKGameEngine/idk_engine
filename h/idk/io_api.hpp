#pragma once

#include "idk/core/double_buffer.hpp"
#include "idk/core/log.hpp"
#include "idk/core/service.hpp"

#include "idk/engine/io_interface.hpp"


namespace idk
{
    class IoService;

    class IoApi
    {
    private:
        using Request = idk::IoReqType;
        idk::IoService *srv_;
        DblBufferWriter<IoRequest> writer_;

        template <Request R>
        void _send_request(const IoRequestImpl<R> &req_impl, IoResponseImpl<R> *res_impl)
        {
            writer_->push( IoRequest::makeIoRequest<R>(req_impl, res_impl) );
        }

    public:
        IoApi(IoService*);

        void dummy() { (void)srv_; }

        // #define IDK_XMACRO(Name) \
        // void Name(const Name ## Request &req, Name ## Response *res) \
        // { \
        //     VLOG_INFO("[IoApi::" #Name "] A"); \
        //     _send_request<Request::Name>(req, res); \
        //     VLOG_INFO("[IoApi::" #Name "] B"); \
        // }
        // IDK_IOREQ_LIST
        // #undef IDK_XMACRO

    };
}
