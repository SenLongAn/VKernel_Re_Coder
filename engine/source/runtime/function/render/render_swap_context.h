#pragma once

#include "runtime/core/math/math_headers.h"

#include <optional>

/**
 * Management of the transmission of logical and rendering data
 * Frame 0 updates data from the logical layer to element 0, swaps the index, and the rendering layer reads data from element 0.
 * Frame 1 updates data from the logical layer to element 1, swaps the index, and the rendering layer reads data from element 1.
 * And so on. Repetitive cycle
 */
namespace VKernel
{
    struct CameraSwapData ///< camera data
    {
        std::optional<float> m_fov_x;
        std::optional<Matrix4x4> m_view_matrix;
    };

    struct RenderSwapData ///< All the data required by the rendering module
    {
        std::optional<CameraSwapData> m_camera_swap_data;
    };

    enum SwapDataType : uint8_t ///< logic or render
    {
        LogicSwapDataType = 0,
        RenderSwapDataType,
        SwapDataTypeCount
    };

    class RenderSwapContext
    {

    public:
        // get
        RenderSwapData &getLogicSwapData();
        RenderSwapData &getRenderSwapData();

        // reset
        void resetCameraSwapData();

        // swap
        void swapLogicRenderData();

    private:
        // index
        uint8_t m_logic_swap_data_index{LogicSwapDataType};
        uint8_t m_render_swap_data_index{RenderSwapDataType};

        // data
        RenderSwapData m_swap_data[SwapDataTypeCount];

    private:
        bool isReadyToSwap() const; ///< If there is rendered data, then return false.
        void swap();                ///< Exchange of the logic and render index
    };
}