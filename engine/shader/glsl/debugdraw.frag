#version 450

#extension GL_GOOGLE_include_directive :enable
// in
layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint textureType;

// out
layout(location = 0) out vec4 outColor;

// in
layout(set = 0, binding = 2) uniform sampler2D texSampler[2];

void main(){
    // out
    outColor = fragColor;
    
    if(textureType != 0 && fragTexCoord.x >= 0.0f && fragTexCoord.y >= 0.0f) // If there is texture
    {
        vec4 tex = texture(texSampler[textureType - 1], fragTexCoord);
        float xi = tex.r;
        outColor = vec4(fragColor.r*xi,fragColor.g*xi,fragColor.b*xi,fragColor.a*xi); // Texture and input color blending
    }
}