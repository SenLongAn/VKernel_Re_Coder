#pragma once

#include "runtime/core/math/math_headers.h"

#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_object.h"
#include "runtime/resource/res_type/global/global_rendering.h"

#include <deque>
#include <optional>

/**
 * Management of the transmission of logical and rendering data
 * Frame 0 updates data from the logical layer to element 0, swaps the index, and the rendering layer reads data from
 * element 0. Frame 1 updates data from the logical layer to element 1, swaps the index, and the rendering layer reads
 * data from element 1. And so on. Repetitive cycle
 */
namespace VKernel
{
    struct CameraSwapData ///< camera data
    {
        std::optional<float>            m_fov_x;
        std::optional<Matrix4x4>        m_view_matrix;
        std::optional<RenderCameraType> m_camera_type;
    };

    struct GameObjectResourceDesc
    {
        std::deque<GameObjectDesc> m_game_object_descs;

        void add(GameObjectDesc& desc); ///< add
        void pop();                     ///< pop

        bool isEmpty() const; ///< is empty

        GameObjectDesc& getNextProcessObject(); ///< get front
    };

    struct RenderSwapData ///< All the data required by the rendering module
    {
        std::optional<CameraSwapData>         m_camera_swap_data;
        std::optional<GameObjectResourceDesc> m_game_object_resource_desc;

        void addDirtyGameObject(GameObjectDesc&& desc);
    };

    enum SwapDataType : uint8_t ///< logic or render
    {
        LogicSwapDataType = 0,
        RenderSwapDataType,
        SwapDataTypeCount
    };

    struct LevelIBLResourceDesc ///< ibl Environment Map
    {
        SkyBoxIrradianceMap m_skybox_irradiance_map;
        SkyBoxSpecularMap   m_skybox_specular_map;
        std::string         m_brdf_map;
    };

    struct LevelColorGradingResourceDesc ///< ColorGrading
    {
        std::string m_color_grading_map;
    };

    struct LevelResourceDesc ///< level resource
    {
        LevelIBLResourceDesc          m_ibl_resource_desc;
        LevelColorGradingResourceDesc m_color_grading_resource_desc;
    };

    class RenderSwapContext
    {

    public:
        // get
        RenderSwapData& getLogicSwapData();
        RenderSwapData& getRenderSwapData();

        // reset
        void resetCameraSwapData();
        void resetGameObjectResourceSwapData();

        // swap
        void swapLogicRenderData();

    private:
        // index
        uint8_t m_logic_swap_data_index {LogicSwapDataType};
        uint8_t m_render_swap_data_index {RenderSwapDataType};

        // data
        RenderSwapData m_swap_data[SwapDataTypeCount];

    private:
        bool isReadyToSwap() const; ///< If there is rendered data, then return false.
        void swap();                ///< Exchange of the logic and render index
    };
} // namespace VKernel