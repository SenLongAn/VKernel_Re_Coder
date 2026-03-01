layout(std140) struct VulkanMeshInstance
{
    highp mat4  model_matrix;
    highp vec3  color;
    highp float _padding_color;
    highp vec3  apply_lighting;
    highp float _padding_apply_lighting;
    highp int   apply_texture;
    highp float _padding_texture_1;
    highp float _padding_texture_2;
    highp float _padding_texture_3;
};