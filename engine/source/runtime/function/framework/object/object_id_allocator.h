#pragma once

#include <atomic>
#include <limits>

/**
 * Game Object id allocator
 */
namespace VKernel
{
    using GObjectID = std::size_t; ///< Type alias

    constexpr GObjectID k_invalid_gobject_id = std::numeric_limits<std::size_t>::max(); ///< invalid id

    class ObjectIDAllocator
    {
    public:
        static GObjectID alloc(); ///< alloc id

    private:
        static std::atomic<GObjectID> m_next_id; ///< id
    };
} // namespace VKernel
