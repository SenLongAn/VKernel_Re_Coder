#pragma once

/**
 * render resource, abstract base class
 */
namespace VKernel
{
    class RenderResourceBase
    {
    
    public:
        virtual ~RenderResourceBase() {} ///< Destructor
        virtual void clear() = 0; ///< clear
    };
}