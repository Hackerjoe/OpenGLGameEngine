#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 Tangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;


    vec3 normal;
    normal = texture(texture_normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(Tangent * normal);

    gNormal = normalize(normal);
    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = vec3(1,1,1); //texture(texture_diffuse1, TexCoords).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = 0.5;//texture(texture_specular1, TexCoords).r;
}
