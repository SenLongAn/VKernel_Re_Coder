#pragma once

#include "runtime/function/render/render_entity.h"
#include "runtime/function/render/render_type.h"

#include <memory>

/**
 * All the data required for rendering, base class
 */
namespace VKernel
{
    class RenderCamera;
    class VulkanAPI;

    class RenderResourceBase
    {
    public:
        virtual ~RenderResourceBase() {}

        virtual void clear() = 0;

        virtual void updatePerFrameBuffer(std::shared_ptr<RenderCamera> camera) = 0;

        // load

        virtual void uploadGlobalRenderResource(std::shared_ptr<VulkanAPI> vulkan_api) = 0;

        virtual void uploadGameObjectRenderResource(std::shared_ptr<VulkanAPI> vulkan_api,
                                                    RenderEntity               render_entity,
                                                    RenderMeshData             mesh_data) = 0;

        std::shared_ptr<TextureData> loadTexture(std::string file, bool is_srgb = false); ///< texture
        RenderMeshData               loadMeshData(const MeshSourceDesc& source);          ///< mesh

    private:
        StaticMeshData loadStaticMesh(std::string mesh_file); ///< mesh
    };
} // namespace VKernel