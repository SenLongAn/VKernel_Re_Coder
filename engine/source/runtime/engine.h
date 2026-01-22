#pragma once

#include <chrono>

/**
 * Engine core
 */

namespace ReCoder
{
    class ReCoderEditor;
}
namespace VKernel
{

    extern bool g_is_editor_mode;
    class VKernelEngine ///< Engine
    {

        friend class ReCoder::ReCoderEditor;

    public:
        void startEngine(const std::string& config_file_path); ///< init
        void run();                                            ///< run
        bool tickOneFrame(float delta_time);                   ///< run
        void shutdownEngine();                                 ///< clear

    protected:
        std::chrono::steady_clock::time_point m_last_tick_time_point {
            std::chrono::steady_clock::now()}; ///< last tick time point

    protected:
        void  logicalTick(float delta_time);  ///< The logical module updates every frame.
        bool  rendererTick(float delta_time); ///< The render module updates every frame.
        float calculateDeltaTime();           ///< calculate DeltaTime，Each frame can only be called once
    };
} // namespace VKernel