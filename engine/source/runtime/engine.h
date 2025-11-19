#pragma once

/**
 * Engine core
 */
namespace VKernel
{
    
    class VKernelEngine ///< Engine
    { 

    public:
        void startEngine(); ///< init
        void run(); ///< run
        void shutdownEngine(); ///< clear
    };
}