#pragma once

/**
 * Editor core
 */
namespace ReCoder{
    
    class ReCoderEditor{ ///< Editor

    public:
        void initialize(); ///< It is executed once at the beginning of the operation.
        void run(); ///< Execute each frame
        void clear(); ///< Execute once at the end of the operation.
    };
}