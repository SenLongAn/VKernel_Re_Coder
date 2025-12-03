#pragma once

/**
 * Manage all systems and managers
 */

namespace VKernel
{
    class WindowSystem;
    class RenderSystem;
    class VKernelEngine;
}

namespace ReCoder
{
    struct EditorGlobalContextInitInfo
    {
        VKernel::WindowSystem *window_system;
        VKernel::RenderSystem *render_system;
        VKernel::VKernelEngine *engine_runtime;
    };

    class EditorGlobalContext
    {
    public:
        class EditorSceneManager *m_scene_manager{nullptr};
        class EditorInputManager *m_input_manager{nullptr};
        VKernel::RenderSystem *m_render_system{nullptr};
        VKernel::WindowSystem *m_window_system{nullptr};
        VKernel::VKernelEngine *m_engine_runtime{nullptr};

    public:
        void initialize(const EditorGlobalContextInitInfo &init_info);
        void clear();
    };

    extern EditorGlobalContext g_editor_global_context;
}