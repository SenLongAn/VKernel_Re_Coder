#pragma once

/**
 * Function registration interface in _generated/reflection
 */

namespace VKernel
{
    namespace Reflection
    {
        class TypeMetaRegister
        {
        public:
            static void metaRegister();
            static void metaUnregister();
        };
    } // namespace Reflection
} // namespace VKernel