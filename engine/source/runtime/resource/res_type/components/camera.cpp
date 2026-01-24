#include "runtime/resource/res_type/components/camera.h"

#include "runtime/core/base/macro.h"

namespace VKernel
{
    CameraComponentRes::CameraComponentRes(const CameraComponentRes& res)
    {
        m_parameter = VKERNEL_REFLECTION_NEW(ThirdPersonCameraParameter);
        VKERNEL_REFLECTION_DEEP_COPY(ThirdPersonCameraParameter, m_parameter, res.m_parameter);
    }

    CameraComponentRes::~CameraComponentRes() { VKERNEL_REFLECTION_DELETE(m_parameter); }
} // namespace VKernel