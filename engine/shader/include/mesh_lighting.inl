// highp vec3 result_color = blinn_phong(L, V, N, objectColor, ambient_light, scene_directional_light.color, 8.0);

// ambient light
highp vec3 La = objectColor * ambient_light;

// point light
highp vec3 F0 = mix(vec3(0.04), objectColor, 0.5);
highp vec3 Lo = vec3(0.0, 0.0, 0.0);

for (highp int light_index = 0; light_index < int(point_light_num) && light_index < m_max_point_light_count;
     ++light_index) ///< iterate
{
    highp vec3  point_light_position = scene_point_lights[light_index].position; ///< position
    highp float point_light_radius   = scene_point_lights[light_index].radius;   ///< radius

    highp vec3  L   = normalize(point_light_position - in_world_position); ///< light Direction
    highp float NoL = min(dot(N, L), 1.0);

    // Calculate attenuation
    highp float distance             = length(point_light_position - in_world_position); ///< distance
    highp float distance_attenuation = 1.0 / (distance * distance + 1.0);                ///< Nonlinear
    highp float radius_attenuation   = 1.0 - ((distance * distance) / (point_light_radius * point_light_radius));
    highp float light_attenuation    = radius_attenuation * distance_attenuation * NoL;
    if (light_attenuation > 0.0) ///< If the attenuation > 0
    {

        highp vec3 En = scene_point_lights[light_index].intensity * light_attenuation;
        Lo += BRDF(L, V, N, F0, objectColor, 0.5, 0.5, En);
    }
};

// directional light
if (dot(N, L) > 0.0)
{
    Lo += BRDF(scene_directional_light.direction, V, N, F0, objectColor, 0.5, 0.5, scene_directional_light.color);
}

result_color = La + Lo;
result_color = filmic(result_color);