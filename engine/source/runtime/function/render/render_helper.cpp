#include "runtime/function/render/render_helper.h"

#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_scene.h"

#include <iomanip>
#include <iostream>

namespace VKernel
{
    BoundingBox BoundingBoxTransform(BoundingBox const& b, Matrix4x4 const& m)
    {
        Vector3 const g_BoxOffset[8] = {Vector3(-1.0f, -1.0f, 1.0f),
                                        Vector3(1.0f, -1.0f, 1.0f),
                                        Vector3(1.0f, 1.0f, 1.0f),
                                        Vector3(-1.0f, 1.0f, 1.0f),
                                        Vector3(-1.0f, -1.0f, -1.0f),
                                        Vector3(1.0f, -1.0f, -1.0f),
                                        Vector3(1.0f, 1.0f, -1.0f),
                                        Vector3(-1.0f, 1.0f, -1.0f)};

        size_t const CORNER_COUNT = 8;

        // center
        Vector3 center((b.max_bound.x + b.min_bound.x) * 0.5,
                       (b.max_bound.y + b.min_bound.y) * 0.5,
                       (b.max_bound.z + b.min_bound.z) * 0.5);

        // extent
        Vector3 extents((b.max_bound.x - b.min_bound.x) * 0.5,
                        (b.max_bound.y - b.min_bound.y) * 0.5,
                        (b.max_bound.z - b.min_bound.z) * 0.5);

        // Apply Transformation
        Vector3 min;
        Vector3 max;

        for (size_t i = 0; i < CORNER_COUNT; ++i)
        {
            Vector3 corner_before = extents * g_BoxOffset[i] + center;
            Vector4 corner_with_w = m * Vector4(corner_before.x, corner_before.y, corner_before.z, 1.0);
            Vector3 corner        = Vector3(corner_with_w.x / corner_with_w.w,
                                     corner_with_w.y / corner_with_w.w,
                                     corner_with_w.z / corner_with_w.w);

            if (0 == i)
            {
                min = corner;
                max = corner;
            }
            else
            {
                min = Vector3(Math::min(min[0], corner[0]), Math::min(min[1], corner[1]), Math::min(min[2], corner[2]));
                max = Vector3(Math::max(max[0], corner[0]), Math::max(max[1], corner[1]), Math::max(max[2], corner[2]));
            }
        }

        BoundingBox b_out;
        b_out.max_bound = max;
        b_out.min_bound = min;
        return b_out;
    }

    Matrix4x4 CalculateDirectionalLightCamera(RenderScene& scene, RenderCamera& camera)
    {
        // Calculate main camera bounding box
        Matrix4x4 proj_view_matrix; ///< main camera PV
        {
            Matrix4x4 view_matrix = camera.getViewMatrix();
            Matrix4x4 proj_matrix = camera.getPersProjMatrix();
            proj_view_matrix      = proj_matrix * view_matrix;
        }

        BoundingBox frustum_bounding_box; ///< main camera bounding box
        {
            Vector3 const g_frustum_points_ndc_space[8] = {Vector3(-1.0f, -1.0f, 1.0f),
                                                           Vector3(1.0f, -1.0f, 1.0f),
                                                           Vector3(1.0f, 1.0f, 1.0f),
                                                           Vector3(-1.0f, 1.0f, 1.0f),
                                                           Vector3(-1.0f, -1.0f, 0.0f),
                                                           Vector3(1.0f, -1.0f, 0.0f),
                                                           Vector3(1.0f, 1.0f, 0.0f),
                                                           Vector3(-1.0f, 1.0f, 0.0f)}; ///< Standard NDC

            Matrix4x4 inverse_proj_view_matrix = proj_view_matrix.inverse(); ///< main camera PV inverse

            frustum_bounding_box.min_bound = Vector3(FLT_MAX, FLT_MAX, FLT_MAX); ///< Initially set to an invalid value
            frustum_bounding_box.max_bound = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);

            size_t const CORNER_COUNT = 8;
            for (size_t i = 0; i < CORNER_COUNT; ++i) ///< Iterate over 8 vertices
            {
                // ndc cube -> p^-1 -> camera frustum(Origin position) -> v^-1 -> camera frustum(homogeneous space)
                Vector4 frustum_point_with_w = inverse_proj_view_matrix * Vector4(g_frustum_points_ndc_space[i].x,
                                                                                  g_frustum_points_ndc_space[i].y,
                                                                                  g_frustum_points_ndc_space[i].z,
                                                                                  1.0);
                // Perspective division, transforming from homogeneous space to world space
                Vector3 frustum_point = Vector3(frustum_point_with_w.x / frustum_point_with_w.w,
                                                frustum_point_with_w.y / frustum_point_with_w.w,
                                                frustum_point_with_w.z / frustum_point_with_w.w);

                // Calculate bounding box
                frustum_bounding_box.merge(frustum_point);
            }
        }

        // Calculate scene bounding box
        BoundingBox scene_bounding_box;
        {
            scene_bounding_box.min_bound = Vector3(FLT_MAX, FLT_MAX, FLT_MAX); ///< Initially set to an invalid value
            scene_bounding_box.max_bound = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);

            for (const RenderEntity& entity : scene.m_render_entities) ///< Iterate through all entities
            {
                BoundingBox mesh_asset_bounding_box {
                    entity.m_bounding_box.getMinCorner(),
                    entity.m_bounding_box.getMaxCorner()}; ///< The bounding box of the entity (local space)

                BoundingBox mesh_bounding_box_world =
                    BoundingBoxTransform(mesh_asset_bounding_box, entity.m_model_matrix); ///< Transform to world space

                scene_bounding_box.merge(mesh_bounding_box_world); ///< Calculate bounding box
            }
        }

        // Calculate the view-projection matrix of the directional light
        // We expect to see shadows everywhere we go. Parallel light is infinitely large and can cover the entire scene.
        // 1· If we specify a fixed position and range, it will only generate shadows in that area.
        // 2· If we fix the position but cover the entire scene, the computation will be very heavy.
        // 3· Keeping its direction unchanged, moving it with the camera, and only rendering the intersection of the
        // camera and the scene can reduce computation while ensuring shadows are rendered in the camera's visible area.
        Matrix4x4 light_view;
        Matrix4x4 light_proj;
        {
            // Main camera bounding box, center, range
            Vector3 box_center((frustum_bounding_box.max_bound.x + frustum_bounding_box.min_bound.x) * 0.5,
                               (frustum_bounding_box.max_bound.y + frustum_bounding_box.min_bound.y) * 0.5,
                               (frustum_bounding_box.max_bound.z + frustum_bounding_box.min_bound.z));
            Vector3 box_extents((frustum_bounding_box.max_bound.x - frustum_bounding_box.min_bound.x) * 0.5,
                                (frustum_bounding_box.max_bound.y - frustum_bounding_box.min_bound.y) * 0.5,
                                (frustum_bounding_box.max_bound.z - frustum_bounding_box.min_bound.z) * 0.5);

            // View matrix of parallel light source
            // box_center moves with the camera, m_direction ensures the direction remains unchanged, and length of the
            // diagonal ensures coverage of the entire camera view.
            Vector3 eye    = box_center + scene.m_directional_light.m_direction * box_extents.length();
            Vector3 center = box_center;
            light_view     = Math::makeLookAtMatrix(eye, center, Vector3(0.0, 1.0, 0.0)); ///< inverse matrix
            // Projection matrix of parallel light source
            BoundingBox frustum_bounding_box_light_view = BoundingBoxTransform(
                frustum_bounding_box, light_view); ///< Transform to light view space to calculate the clipping region
            BoundingBox scene_bounding_box_light_view = BoundingBoxTransform(scene_bounding_box, light_view);

            light_proj = Math::makeOrthographicProjectionMatrix01( ///< Take the intersection
                std::max(frustum_bounding_box_light_view.min_bound.x, scene_bounding_box_light_view.min_bound.x),
                std::min(frustum_bounding_box_light_view.max_bound.x, scene_bounding_box_light_view.max_bound.x),
                std::max(frustum_bounding_box_light_view.min_bound.y, scene_bounding_box_light_view.min_bound.y),
                std::min(frustum_bounding_box_light_view.max_bound.y, scene_bounding_box_light_view.max_bound.y),
                -scene_bounding_box_light_view.max_bound
                     .z, // the objects which are nearer than the frustum bounding box may caster shadow as well
                -std::max(frustum_bounding_box_light_view.min_bound.z, scene_bounding_box_light_view.min_bound.z));
        }

        Matrix4x4 light_proj_view = (light_proj * light_view);
        return light_proj_view;
    }

    std::vector<std::vector<Matrix4x4>> CalculatePointLightCamera(RenderScene& scene)
    {
        uint8_t                             n = scene.m_point_light_list.m_lights.size();
        std::vector<std::vector<Matrix4x4>> res(n);

        const Vector3 targets[6] = {Vector3(1.0f, 0.0f, 0.0f),
                                    Vector3(-1.0f, 0.0f, 0.0f),
                                    Vector3(0.0f, 1.0f, 0.0f),
                                    Vector3(0.0f, -1.0f, 0.0f),
                                    Vector3(0.0f, 0.0f, 1.0f),
                                    Vector3(0.0f, 0.0f, -1.0f)};

        const Vector3 ups[6] = {Vector3(0.0f, -1.0f, 0.0f),
                                Vector3(0.0f, -1.0f, 0.0f),
                                Vector3(0.0f, 0.0f, 1.0f),
                                Vector3(0.0f, 0.0f, -1.0f),
                                Vector3(0.0f, -1.0f, 0.0f),
                                Vector3(0.0f, -1.0f, 0.0f)};

        Radian fovy(Math::degreesToRadians(90.0f));
        float  aspect = 1.0f;
        float  znear  = 0.01f;
        float  zfar   = 25.0f;

        for (int i = 0; i < n; i++)
        {
            const PointLight& light     = scene.m_point_light_list.m_lights[i];
            Vector3           light_pos = light.m_position;
            zfar                        = light.calculateRadius();

            // proj
            Matrix4x4 light_proj = Math::makePerspectiveMatrix(fovy, aspect, znear, zfar);

            for (size_t face = 0; face < 6; face++)
            {
                // view
                Vector3   target     = light_pos + targets[face];
                Matrix4x4 light_view = Math::makeLookAtMatrix(light_pos, target, ups[face]);

                // VP
                Matrix4x4 light_proj_view = (light_proj * light_view);
                res[i].push_back(light_proj_view);
            }
        }

        return res;
    }
} // namespace VKernel