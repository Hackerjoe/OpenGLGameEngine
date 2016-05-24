#version 410 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform samplerCube environmentMap;
uniform sampler2D secondSum;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
};

uniform vec3 viewPos;
uniform Light light;
uniform float roughnessValue;
uniform float F0;
uniform float k;
uniform vec2 screensize;
uniform float metal;


vec2 CalcTexCoord()
{
    return gl_FragCoord.xy/(screensize);
}

// GGX Geometric Shadowing using Smith's Method
float GGXSmith(float NDotL, float NDotV, float aSquared)
{
    float a = sqrt(aSquared);
    float k = ((a + 1) * (a +1))/8;
    float G_V = NDotV / (NDotV * (1-k) +k);
    float G_L = NDotL / (NDotL * (1-k) +k);

    return G_L*G_V;
}

float GGXNDF(float NDotH,float aSquared)
{
    // GGX (Trowbridge-Reitz) Normal Distribution Function
    float NdotHSquared = NDotH * NDotH;
    float i = (NdotHSquared*(aSquared-1)+1);
    i = i * i;
    float denominator = 3.14*i;
    return aSquared/denominator;

}


// fresnel
// Schlick approximation
float FresnelSchlick(float VDotH)
{
    float fresnel = pow(1.0 - VDotH, 5.0);
    fresnel *= (1.0 - F0);
    fresnel += F0;

    return fresnel;
}

float saturate (float x) {
	return clamp(x, 0.0, 1.0);
}

vec3 saturate (vec3 v) {
	return clamp(v, 0.0, 1.0);
}

float ComputeCubemapMipFromRoughness( float Roughness, float MipCount ){
	// Level starting from 1x1 mip
	float Level = 3 - 1.15 * log2( Roughness );
	return MipCount - 1 - Level;
}

void main()
{
    vec3 FragPos = texture(gPosition, CalcTexCoord()).rgb;
    vec3 Normal = texture(gNormal, CalcTexCoord()).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, CalcTexCoord()).rgb;
    float Specular = texture(gAlbedoSpec, CalcTexCoord()).a;

    float distance = length(light.Position - FragPos);
    vec3 lighting = vec3(0,0,0);
    if(distance < 1)
    {


        vec3 normal = normalize(Normal);

        vec3 lightDirection = normalize(light.Position - FragPos);

        float NdotL = max(dot(normal, lightDirection), 0.0);

        vec3 spec;

        vec3 diffuse;

        if(NdotL > 0.0)
        {
            vec3 eyeDir = viewPos;

            // Lambertian Diffuse Reflection
            //TODO multiply by intensity
            diffuse = max(dot(Normal, lightDirection), 0.0) * Diffuse * light.Color * 2;

            // calculate intermediary values
            vec3 halfVector = normalize(lightDirection + eyeDir);
            //clamp(dot(N,H),0,1);
            float NdotH = clamp(dot(normal, halfVector),0.0, 1.0);
            float NdotV = clamp(dot(normal, eyeDir),0.0, 1.0);
            float VdotH = clamp(dot(eyeDir, halfVector),0.0, 1.0);
            float mSquared = roughnessValue * roughnessValue;


            float Rs = (FresnelSchlick(VdotH) * GGXSmith(NdotL,NdotV,mSquared) * GGXNDF(NdotH,mSquared)) / (NdotV * NdotL * 3.14);
            //spec = Specular * (light.Color * NdotL * (k + Rs * (1.0 - k)));

            vec3 I = normalize(FragPos - viewPos);
            vec3 R = reflect(I, normalize(Normal));

            //lighting +=   diffuse;

        }
        // attenuation of light
        // fall off of light
        float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
        float miplevel = ComputeCubemapMipFromRoughness(roughnessValue,8);
        vec3 eyeDir = viewPos;
        float NoV = saturate( dot( normal, eyeDir ) );
        vec3 R = 2 * dot( eyeDir, normal ) * normal - eyeDir;

        vec3 specular_rad		= textureLod(environmentMap, R, metal).rgb;
        vec2 f0_scale_bias		= texture(secondSum, vec2(NoV, roughnessValue)).rg;

        vec2 AB =  texture2D(secondSum, vec2(NoV, roughnessValue), 0).rg;
	      vec3 GF = light.Color * AB.x + saturate( 50.0 * light.Color.g ) * AB.y;
	      //vec3 GF = SpecularColor * AB.x + AB.y;


        lighting.rgb += specular_rad*GF;

        //lighting *= attenuation;
    }


    FragColor = vec4(lighting, 1.0);
}
