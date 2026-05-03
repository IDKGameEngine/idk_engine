#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <glm/glm.hpp>

#include "idk/core/metric.hpp"


namespace idk
{
    class  IoService;
    class  IoApi;
    struct IoRequest;
    struct IoResponse;

    #define IDK_IOREQ_LIST \
        IDK_XMACRO(LoadFile) \
        IDK_XMACRO(WriteFile)

    enum class IoReqType: uint64_t
    {
        Invalid = 0,

        #define IDK_XMACRO(Name) Name,
        IDK_IOREQ_LIST
        #undef IDK_XMACRO

        NumTypes
    };


    struct IoResponse
    {
    private:
        friend class IoService;
        std::atomic_bool ready_;
        void make_ready() { ready_.store(true); }        

    public:
        IoResponse(): ready_(false) {  };
        bool is_ready() { return ready_.load(); }
        void await_ready() { while (!is_ready()) {  } }
    };

    template <IoReqType R>
    struct IoRequestImpl;

    template <IoReqType R>
    struct IoResponseImpl;


    template<>
    struct IoRequestImpl<IoReqType::LoadFile>
    {
        const char *path;
        IoRequestImpl(const char *filepath): path(filepath) {  }
    };
    template<>
    struct IoResponseImpl<IoReqType::LoadFile>: public IoResponse
    {
        int64_t id;
        IoResponseImpl(): id(-1) { }
    };


    template<>
    struct IoRequestImpl<IoReqType::WriteFile>
    {
        const char *comp_path;
        IoRequestImpl(const char *cpath): comp_path(cpath) {  }
    };
    template<>
    struct IoResponseImpl<IoReqType::WriteFile>: public IoResponse
    {
        int64_t id;
        IoResponseImpl(): id(-1) { }
    };


    #define IDK_XMACRO(Name) using Name ## Request = IoRequestImpl<IoReqType::Name>;
    IDK_IOREQ_LIST
    #undef IDK_XMACRO

    #define IDK_XMACRO(Name) using Name ## Response = IoResponseImpl<IoReqType::Name>;
    IDK_IOREQ_LIST
    #undef IDK_XMACRO

}



struct idk::IoRequest
{
public:
    IoReqType  type;
    IoResponse *res;

    union {
        char as_bytes[0];

        #define IDK_XMACRO(Name) Name ## Request as_##Name;
        IDK_IOREQ_LIST
        #undef IDK_XMACRO
    };

    IoRequest(): type(IoReqType::Invalid), res(nullptr) {  }

    template <IoReqType R>
    static IoRequest makeIoRequest(const IoRequestImpl<R> &req_impl, IoResponseImpl<R> *res_impl)
    {
        IoRequest req;

        req.type = R;
        req.res  = res_impl;
        std::memcpy(req.as_bytes, &req_impl, sizeof(IoRequestImpl<R>));

        return req;
    }
};


