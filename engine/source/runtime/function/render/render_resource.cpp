#include "runtime/function/render/render_resource.h"

#include "runtime/function/render/render_camera.h"
#include "render_resource.h"

namespace VKernel
{
    void RenderResource::clear()
    {
    }

    void RenderResource::updatePerFrameBuffer(std::shared_ptr<RenderCamera> camera)
    {
        Matrix4x4 view_matrix = camera->getViewMatrix();
        Matrix4x4 proj_matrix = camera->getPersProjMatrix();
        Vector3 camera_position = camera->position();
        Matrix4x4 proj_view_matrix = proj_matrix * view_matrix;

        m_mesh_perframe_storage_buffer_object.proj_view_matrix = proj_view_matrix;
        m_mesh_perframe_storage_buffer_object.camera_position = camera_position;
    }
}