#include "runtime/engine.h"

#include "runtime/function/global/global_context.h"

namespace VKernel
{

    void VKernelEngine::startEngine()
    {
        g_runtime_global_context.startSystems();
    }
    
    void VKernelEngine::run(){}
    
    void VKernelEngine::shutdownEngine()
    {
        g_runtime_global_context.shutdownSystems();
    }
}