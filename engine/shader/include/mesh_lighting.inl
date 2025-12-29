// highp vec3 result_color = blinn_phong(L, V, N, objectColor, ambient_light, scene_directional_light.color, 8.0);

// ambient light
highp vec3 La = objectColor * ambient_light;

// Indirect lighting
highp vec3 origin_samplecube_N = vec3(N.x, -N.y, N.z);
highp vec3 R                   = reflect(-V, N);
highp vec3 origin_samplecube_R = vec3(R.x, -R.y, R.z);

highp vec3 irradiance = texture(irradiance_sampler, origin_samplecube_N).rgb;
highp vec3 diffuse    = irradiance * objectColor; ///< L * F

highp vec3  F0         = mix(vec3(0.04), objectColor, metallic);
highp vec3  F          = F_SchlickR(clamp(dot(N, V), 0.0, 1.0), F0, roughness);                    ///< Fresnel
highp vec2  brdfLUT    = texture(brdfLUT_sampler, vec2(clamp(dot(N, V), 0.0, 1.0), roughness)).rg; ///< brdfLUT
highp float lod        = roughness * MAX_REFLECTION_LOD;                                           ///< mipmap level
highp vec3  reflection = textureLod(specular_sampler, origin_samplecube_R, lod).rgb;               ///< L
highp vec3  specular   = reflection * (F * brdfLUT.x + brdfLUT.y);                                 // L * F

highp vec3 kD = 1.0 - F; ///< weight
kD *= 1.0 - metallic;
highp vec3 Libl = (kD * diffuse + specular);

// point light
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
        Lo += BRDF(L, V, N, F0, objectColor, metallic, roughness, En);
    }
};

// directional light
if (dot(N, L) > 0.0)
{
    Lo += BRDF(
        scene_directional_light.direction, V, N, F0, objectColor, metallic, roughness, scene_directional_light.color);
}

result_color = La + Lo + Libl;
result_color = filmic(result_color);