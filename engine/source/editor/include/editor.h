#pragma once

/**
 * Editor core
 */
namespace VKernel {
    class VKernelEngine;
}
namespace ReCoder
{
    class ReCoderEditor ///< Editor
    { 

    public:
        void initialize(VKernel::VKernelEngine* engine_runtime); ///< init
        void run(); ///< run
        void clear(); ///< clear

    private:
        VKernel::VKernelEngine* m_engine_runtime{ nullptr };
    };
}