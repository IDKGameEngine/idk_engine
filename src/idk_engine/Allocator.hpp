#pragma once

#include "idk/core/metric.hpp"
#include "idk/core/Allocator.hpp"

namespace idk::engine
{
    static constexpr size_t ALLOCATOR_SIZE = 1 * idk::MEGA;

    template <typename T, typename... Args>
    static inline T *New(Args&&... args)
    {
        static idk::core::BumpAllocator<engine::ALLOCATOR_SIZE> instance;
        return instance.New<T>(args...);
    }
}
