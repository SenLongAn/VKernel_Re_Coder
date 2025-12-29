/**
 * Lighting Model
 */

#define PI 3.1416
#define MAX_REFLECTION_LOD 8.0

//  Normal Distribution function --------------------------------------
highp float D_GGX(highp float dotNH, highp float roughness)
{
    highp float alpha  = roughness * roughness;
    highp float alpha2 = alpha * alpha;
    highp float denom  = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
    return (alpha2) / (PI * denom * denom);
}

//  Geometric Shadowing function --------------------------------------
highp float G_SchlicksmithGGX(highp float dotNL, highp float dotNV, highp float roughness)
{
    highp float r  = (roughness + 1.0);
    highp float k  = (r * r) / 8.0;
    highp float GL = dotNL / (dotNL * (1.0 - k) + k);
    highp float GV = dotNV / (dotNV * (1.0 - k) + k);
    return GL * GV;
}

// Fresnel function ----------------------------------------------------
highp float Pow5(highp float x) { return (x * x * x * x * x); }

highp vec3 F_Schlick(highp float cosTheta, highp vec3 F0)
{
    return F0 + (1.0 - F0) * Pow5(clamp(1.0 - cosTheta, 0.0, 1.0));
}

highp vec3 F_SchlickR(highp float cosTheta, highp vec3 F0, highp float roughness)
{
    return F0 + (max(vec3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * Pow5(1.0 - cosTheta);
}

// Rendering Equation --------------------------------------------
highp vec3 BRDF(highp vec3  L,
                highp vec3  V,
                highp vec3  N,
                highp vec3  F0,
                highp vec3  basecolor,
                highp float metallic,
                highp float roughness,
                highp vec3  lightColor)
{
    highp vec3 H = normalize(V + L);

    highp float dotNV = clamp(dot(N, V), 0.0, 1.0);
    highp float dotNL = clamp(dot(N, L), 0.0, 1.0);
    highp float dotLH = clamp(dot(L, H), 0.0, 1.0);
    highp float dotNH = clamp(dot(N, H), 0.0, 1.0);
    highp float dotHV = clamp(dot(H, V), 0.0, 1.0);

    highp float rroughness = max(0.05, roughness);

    highp float D = D_GGX(dotNH, rroughness);
    highp float G = G_SchlicksmithGGX(dotNL, dotNV, rroughness);
    highp vec3  F = F_Schlick(dotHV, F0);

    highp vec3 spec = D * F * G / (4.0 * dotNL * dotNV + 0.001);

    highp vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

    highp vec3 color = vec3(0.0);
    color += (kD * basecolor / PI + (1.0 - kD) * spec) * lightColor * dotNL;

    return color;
}

// tone --------------------------------------------
vec3 reinhard(vec3 color) { return color / (color + 1.0); }

vec3 acesFilm(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 filmic(vec3 x)
{
    vec3 X      = max(vec3(0.0), x - 0.004);
    vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
    return pow(result, vec3(2.2));
}

// blinn_phong --------------------------------------------
highp vec3 blinn_phong(highp vec3  L,
                       highp vec3  V,
                       highp vec3  N,
                       highp vec3  objectColor,
                       highp vec3  ambientLight,
                       highp vec3  lightColor,
                       highp float shininess)
{
    highp vec3 color = vec3(0.0);

    // ambient
    highp vec3 ambient = lightColor * ambientLight;

    // diffuse
    highp float diff    = max(dot(N, L), 0.0);
    highp vec3  diffuse = diff * lightColor * 0.5;

    // specular
    highp vec3  H        = normalize(V + L);
    highp float spec     = pow(max(dot(N, H), 0.0), shininess);
    highp vec3  specular = spec * lightColor * 1.0;

    color = (ambient + diffuse + specular) * objectColor;
    return color;
}