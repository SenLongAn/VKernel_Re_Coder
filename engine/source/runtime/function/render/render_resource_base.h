#pragma once

#include "runtime/function/render/render_type.h"

#include <memory>

/**
 * All the data required for rendering, base class
 */
namespace VKernel
{
    class RenderCamera;

    class RenderResourceBase
    {
    public:
        virtual ~RenderResourceBase() {}

        virtual void clear() = 0;

        virtual void updatePerFrameBuffer(std::shared_ptr<RenderCamera> camera) = 0;

        std::shared_ptr<TextureData> loadTexture(std::string file, bool is_srgb = false); ///< load texture
    };
}