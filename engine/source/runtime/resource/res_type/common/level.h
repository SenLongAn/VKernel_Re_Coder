#pragma once
#include "runtime/core/meta/reflection/reflection.h"

#include "runtime/core/math/vector3.h"

#include "runtime/resource/res_type/common/object.h"

namespace VKernel
{
    REFLECTION_TYPE(LevelRes)
    CLASS(LevelRes, Fields)
    {
        REFLECTION_BODY(LevelRes);

    public:
        std::vector<ObjectInstanceRes> m_objects;
    };
} // namespace VKernel