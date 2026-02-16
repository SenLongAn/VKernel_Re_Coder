#include "editor/include/editor.h"

#include "runtime/engine.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include "editor/include/editor_global_context.h"
#include "editor/include/editor_input_manager.h"
#include "editor/include/editor_scene_manager.h"
#include "editor/include/editor_ui.h"

namespace ReCoder
{

    void ReCoderEditor::registerEdtorTickComponent(std::string component_type_name)
    {
        VKernel::g_editor_tick_component_types.insert(component_type_name);
    }

    ReCoderEditor::ReCoderEditor()
    {
        registerEdtorTickComponent("TransformComponent");
        registerEdtorTickComponent("MeshComponent");
    }

    void ReCoderEditor::initialize(VKernel::VKernelEngine* engine_runtime)
    {
        VKernel::g_is_editor_mode = true;
        m_engine_runtime          = engine_runtime;

        // init global context
        EditorGlobalContextInitInfo init_info = {VKernel::g_runtime_global_context.m_window_system.get(),
                                                 VKernel::g_runtime_global_context.m_render_system.get(),
                                                 engine_runtime};
        g_editor_global_context.initialize(init_info);

        // camera
        g_editor_global_context.m_scene_manager->setEditorCamera(
            VKernel::g_runtime_global_context.m_render_system->getRenderCamera());
        g_editor_global_context.m_scene_manager->uploadAxisResource();
    }

    void ReCoderEditor::run()
    {
        float delta_time;
        while (true)
        {
            delta_time = m_engine_runtime->calculateDeltaTime();
            g_editor_global_context.m_input_manager->tick(delta_time);
            if (!m_engine_runtime->tickOneFrame(delta_time))
                return;
        }
    }

    void ReCoderEditor::clear() {}
} // namespace ReCoder