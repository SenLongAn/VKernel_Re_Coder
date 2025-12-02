#pragma once

#include "runtime/function/render/render_resource_base.h"
#include "runtime/function/render/render_common.h"

#include <memory>

/**
 * All the data required for rendering
 */
namespace VKernel
{
    class RenderCamera;

    class RenderResource : public RenderResourceBase
    {
    public:
        MeshPerframeStorageBufferObject m_mesh_perframe_storage_buffer_object;

        virtual void updatePerFrameBuffer(std::shared_ptr<RenderCamera> camera) override final;
    };
}