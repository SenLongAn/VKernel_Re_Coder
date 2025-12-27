// highp vec3 result_color = blinn_phong(L, V, N, objectColor, ambient_light, scene_directional_light.color, 8.0);

highp vec3 La = objectColor * ambient_light;

highp vec3 F0 = mix(vec3(0.04), objectColor, 0.5);
highp vec3 Lo = vec3(0.0, 0.0, 0.0);

if (dot(N, L) > 0.0)
{
    Lo += BRDF(scene_directional_light.direction, V, N, F0, objectColor, 0.5, 0.5, scene_directional_light.color);
}

result_color = La + Lo;
result_color = filmic(result_color);