#pragma once

/**
 * Engine core
 */
namespace VKernel{
    
    class VKernelEngine{ ///< Engine

    public:
        void initialize(); ///< It is executed once at the beginning of the operation.
        void run(); ///< Execute each frame
        void clear(); ///< Execute once at the end of the operation.
    };
}