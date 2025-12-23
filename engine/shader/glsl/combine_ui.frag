#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

// input attachment
layout(input_attachment_index = 0, set = 0, binding = 0) uniform highp subpassInput in_scene_color; // secne texture

layout(input_attachment_index = 1, set = 0, binding = 1) uniform highp subpassInput in_ui_color; // editor ui texture

layout(location = 0) out highp vec4 out_color;

void main()
{
    // sampler
    highp vec4 scene_color = subpassLoad(in_scene_color).rgba;

    highp vec4 ui_color = subpassLoad(in_ui_color).rgba;

    // Whether it is transparent, and whether each channel component is less than 0.000001
    if (ui_color.r < 1e-6 && ui_color.g < 1e-6 && ui_color.a < 1e-6) // If the UI is transparent, show the scene;
    {
        // Gamma correct
        ui_color  = vec4(pow(ui_color.r, 1.0 / 2.2),
                        pow(ui_color.g, 1.0 / 2.2),
                        pow(ui_color.b, 1.0 / 2.2),
                        pow(ui_color.a, 1.0 / 2.2));
        out_color = scene_color;
    }
    else // If the scene is transparent, show the UI;
    {
        ui_color  = vec4(pow(ui_color.r, 1.0 / 2.2),
                        pow(ui_color.g, 1.0 / 2.2),
                        pow(ui_color.b, 1.0 / 2.2),
                        pow(ui_color.a, 1.0 / 2.2));
        out_color = ui_color;
    }
}
