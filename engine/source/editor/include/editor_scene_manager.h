#pragma once

#include <memory>

namespace VKernel
{
    class RenderCamera;
}
/**
 * editor manager
 */
namespace ReCoder
{
    class EditorSceneManager
    {
    public:
        void initialize(); ///< init

        // set
        void setEditorCamera(std::shared_ptr<VKernel::RenderCamera> camera) { m_camera = camera; }

        // get
        std::shared_ptr<VKernel::RenderCamera> getEditorCamera() { return m_camera; };

    private:
        std::shared_ptr<VKernel::RenderCamera> m_camera;
    };
}