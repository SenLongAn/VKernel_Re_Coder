#include "runtime/function/framework/object/object_id_allocator.h"

#include <stdexcept>

namespace VKernel
{
    std::atomic<GObjectID> ObjectIDAllocator::m_next_id {0};

    GObjectID ObjectIDAllocator::alloc()
    {
        m_next_id++; ///< add

        std::atomic<GObjectID> new_object_ret = m_next_id.load(); ///< new id

        if (m_next_id >= k_invalid_gobject_id) ///< If the ID exceeds the valid range
        {
            throw std::runtime_error("gobject id overflow");
        }

        return new_object_ret;
    }

} // namespace VKernel
