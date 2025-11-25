#pragma once

#include <chrono>

/**
 * Engine core
 */

namespace ReCoder {
    class ReCoderEditor;
}
namespace VKernel
{
    
    class VKernelEngine ///< Engine
    { 

        friend class ReCoder::ReCoderEditor;

    public:
        void startEngine(); ///< init
        void run(); ///< run
        bool tickOneFrame(float delta_time); ///< run
        void shutdownEngine(); ///< clear
    
    protected:
        std::chrono::steady_clock::time_point m_last_tick_time_point {std::chrono::steady_clock::now()}; ///< last tick time point

    protected:
        bool rendererTick(float delta_time); ///< The render module updates every frame.
        float calculateDeltaTime(); ///< calculate DeltaTime，Each frame can only be called once
    };
}