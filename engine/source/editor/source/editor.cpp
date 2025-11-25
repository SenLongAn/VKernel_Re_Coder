#include "editor/include/editor.h"

#include "runtime/engine.h"

namespace ReCoder
{

    void ReCoderEditor::initialize(VKernel::VKernelEngine* engine_runtime)
    {
        m_engine_runtime = engine_runtime;
    }

    void ReCoderEditor::run()
    {
        float delta_time;
        while (true)
        {
            delta_time = m_engine_runtime->calculateDeltaTime();
            m_engine_runtime->tickOneFrame(delta_time);
        }
    }

    void ReCoderEditor::clear(){}
}