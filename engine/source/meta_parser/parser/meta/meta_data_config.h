#pragma once

/**
 * Attributes of Metadata information
 */

namespace NativeProperty
{
    // Control reflex range
    const auto All = "All"; ///< Reflect all members (fields and methods)

    const auto Fields  = "Fields";  ///< Reflect all fields
    const auto Methods = "Methods"; ///< Reflect all Methods

    const auto WhiteListFields  = "WhiteListFields";  ///< Only reflect the marked fields
    const auto WhiteListMethods = "WhiteListMethods"; ///< Only reflect the marked Methods

    // Control Reflex Switch
    const auto Enable  = "Enable";  ///< Enable reflection
    const auto Disable = "Disable"; ///< Disable reflection

} // namespace NativeProperty
