#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"
precision highp float;
precision highp int;

layout(set = 0, binding = 0) uniform sampler2D in_color;

layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 out_color;

// pixel index in 3*3 kernel
#define UP_LEFT 0
#define UP 1
#define UP_RIGHT 2
#define LEFT 3
#define CENTER 4
#define RIGHT 5
#define DOWN_LEFT 6
#define DOWN 7
#define DOWN_RIGHT 8

// pixel offset in 3*3 kernel
vec2 KERNEL_STEP_MAT[] = vec2[9](vec2(-1.0, 1.0),
                                 vec2(0.0, 1.0),
                                 vec2(1.0, 1.0),
                                 vec2(-1.0, 0.0),
                                 vec2(0.0, 0.0),
                                 vec2(1.0, 0.0),
                                 vec2(-1.0, -1.0),
                                 vec2(0.0, -1.0),
                                 vec2(1.0, -1.0));

// Performance Optimization
#define STEP_COUNT_MAX 12
float QUALITY(int i)
{
    if (i < 5)
        return 1.0;
    if (i == 5)
        return 1.5;
    if (i < 10)
        return 2.0;
    if (i == 10)
        return 4.0;
    if (i == 11)
        return 8.0;
}

// Convert RGB color values to brightness values
float RGB2LUMA(vec3 color) { return dot(vec3(0.299, 0.578, 0.114), color); }

// define
#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define SUBPIXEL_QUALITY 0.75
#define GRADIENT_SCALE 0.25

void main()
{
    // Calculate the pixel brightness values of a 3*3 kernel
    highp ivec2 screen_size = textureSize(in_color, 0);
    highp vec2  uv_step     = vec2(1.0 / float(screen_size.x), 1.0 / float(screen_size.y));
    float       luma_mat[9];
    for (int i = 0; i < 9; i++)
    {
        luma_mat[i] = RGB2LUMA(texture(in_color, in_uv + uv_step * KERNEL_STEP_MAT[i]).xyz);
    }

    // Calculate whether it is a boundary
    float luma_min =
        min(luma_mat[CENTER], min(min(luma_mat[UP], luma_mat[DOWN]), min(luma_mat[LEFT], luma_mat[RIGHT])));
    float luma_max =
        max(luma_mat[CENTER], max(max(luma_mat[UP], luma_mat[DOWN]), max(luma_mat[LEFT], luma_mat[RIGHT])));
    float luma_range = luma_max - luma_min;
    if (luma_range < max(EDGE_THRESHOLD_MIN, luma_max * EDGE_THRESHOLD_MAX))
    {
        out_color = texture(in_color, in_uv);
        return;
    }

    // Calculate Mixed Direction
    float luma_horizontal = abs(luma_mat[UP_LEFT] + luma_mat[DOWN_LEFT] - 2.0 * luma_mat[LEFT]) +
                            2.0 * abs(luma_mat[UP] + luma_mat[DOWN] - 2.0 * luma_mat[CENTER]) +
                            abs(luma_mat[UP_RIGHT] + luma_mat[DOWN_RIGHT] - 2.0 * luma_mat[RIGHT]);
    float luma_vertical = abs(luma_mat[UP_LEFT] + luma_mat[UP_RIGHT] - 2.0 * luma_mat[UP]) +
                          2.0 * abs(luma_mat[LEFT] + luma_mat[RIGHT] - 2.0 * luma_mat[CENTER]) +
                          abs(luma_mat[DOWN_LEFT] + luma_mat[DOWN_RIGHT] - 2.0 * luma_mat[DOWN]);
    bool is_horizontal = luma_horizontal > luma_vertical;

    float gradient_down_left = (is_horizontal ? luma_mat[DOWN] : luma_mat[LEFT]) - luma_mat[CENTER];
    float gradient_up_right  = (is_horizontal ? luma_mat[UP] : luma_mat[RIGHT]) - luma_mat[CENTER];
    bool  is_down_left       = abs(gradient_down_left) > abs(gradient_up_right);

    vec2 step_tangent = (is_horizontal ? vec2(1.0, 0.0) : vec2(0.0, 1.0)) * uv_step;
    vec2 step_normal  = (is_down_left ? -1.0 : 1.0) * (is_horizontal ? vec2(0.0, 1.0) : vec2(1.0, 0.0)) * uv_step;

    // Calculate mixing coefficient
    float gradient = is_down_left ? gradient_down_left : gradient_up_right;

    vec2  uv_start           = in_uv + 0.5 * step_normal;
    float luma_average_start = luma_mat[CENTER] + 0.5 * gradient;
    vec2  uv_pos             = uv_start + step_tangent;
    vec2  uv_neg             = uv_start - step_tangent;

    float delta_luma_pos = RGB2LUMA(texture(in_color, uv_pos).rgb) - luma_average_start;
    float delta_luma_neg = RGB2LUMA(texture(in_color, uv_neg).rgb) - luma_average_start;

    bool reached_pos  = abs(delta_luma_pos) > GRADIENT_SCALE * abs(gradient);
    bool reached_neg  = abs(delta_luma_neg) > GRADIENT_SCALE * abs(gradient);
    bool reached_both = reached_pos && reached_neg;

    if (!reached_pos)
        uv_pos += step_tangent;
    if (!reached_neg)
        uv_neg -= step_tangent;

    if (!reached_both)
    {
        for (int i = 2; i < STEP_COUNT_MAX; i++)
        {
            if (!reached_pos)
                delta_luma_pos = RGB2LUMA(texture(in_color, uv_pos).rgb) - luma_average_start;
            if (!reached_neg)
                delta_luma_neg = RGB2LUMA(texture(in_color, uv_neg).rgb) - luma_average_start;

            bool reached_pos  = abs(delta_luma_pos) > GRADIENT_SCALE * abs(gradient);
            bool reached_neg  = abs(delta_luma_neg) > GRADIENT_SCALE * abs(gradient);
            bool reached_both = reached_pos && reached_neg;

            if (!reached_pos)
                uv_pos += (QUALITY(i) * step_tangent);
            if (!reached_neg)
                uv_neg -= (QUALITY(i) * step_tangent);

            if (reached_both)
                break;
        }
    }

    float length_pos  = max(abs(uv_pos - uv_start).x, abs(uv_pos - uv_start).y);
    float length_neg  = max(abs(uv_neg - uv_start).x, abs(uv_neg - uv_start).y);
    bool  is_pos_near = length_pos < length_neg;

    float pixel_offset = -1.0 * (is_pos_near ? length_pos : length_neg) / (length_pos + length_neg) + 0.5;

    if (((is_pos_near ? delta_luma_pos : delta_luma_neg) < 0.0) == (luma_mat[CENTER] < luma_average_start))
        pixel_offset = 0.0;

    float luma_average_center  = 0.0;
    float average_weight_mat[] = float[9](1.0, 2.0, 1.0, 2.0, 0.0, 2.0, 1.0, 2.0, 1.0);
    for (int i = 0; i < 9; i++)
        luma_average_center += average_weight_mat[i] * luma_mat[i];
    luma_average_center /= 12.0;

    float subpixel_luma_range = clamp(abs(luma_average_center - luma_mat[CENTER]) / luma_range, 0.0, 1.0);
    float subpixel_offset     = (-2.0 * subpixel_luma_range + 3.0) * subpixel_luma_range * subpixel_luma_range;
    subpixel_offset           = subpixel_offset * subpixel_offset * SUBPIXEL_QUALITY;

    pixel_offset = max(pixel_offset, subpixel_offset);

    // Color Mixing
    out_color = texture(in_color, in_uv + pixel_offset * step_normal);
}
