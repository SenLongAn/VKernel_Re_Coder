#pragma once

#include <memory>

/**
 * Editor core
 */
namespace VKernel
{
    class VKernelEngine;
}
namespace ReCoder
{
    class EditorUI;

    class ReCoderEditor ///< Editor
    {

    public:
        void initialize(VKernel::VKernelEngine* engine_runtime); ///< init
        void run();                                              ///< run
        void clear();                                            ///< clear

    private:
        std::shared_ptr<EditorUI> m_editor_ui;
        VKernel::VKernelEngine*   m_engine_runtime {nullptr};
    };
} // namespace ReCoder