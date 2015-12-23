#version 410 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform samplerCube Panorama;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float intensity;
};

uniform vec3 viewPos;
uniform Light light;
uniform float roughnessValue;
uniform float F0;
uniform float k;
uniform vec2 screensize;


vec2 CalcTexCoord()
{

    return gl_FragCoord.xy/(screensize);
}



/* Blinn Phong
 void main()
 {

 // Retrieve data from gbuffer
 vec3 FragPos = texture(gPosition, CalcTexCoord()).rgb;
 vec3 Normal = texture(gNormal, CalcTexCoord()).rgb;
 vec3 Diffuse = texture(gAlbedoSpec, CalcTexCoord()).rgb;
 float Specular = texture(gAlbedoSpec, CalcTexCoord()).a;

 vec3 lighting  = Diffuse * 0.0; // hard-coded ambient component
 vec3 viewDir  = normalize(viewPos - FragPos);
 float distance = length(light.Position - FragPos);

 if(distance < 1)
 {

 // Diffuse
 vec3 lightDir = normalize(light.Position - FragPos);
 vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;

 // Specular
 vec3 halfwayDir = normalize(lightDir + viewDir);
 float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
 vec3 specular = light.Color * spec * Specular;

 // Attenuation
 float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
 diffuse *= attenuation;
 specular *= attenuation;
 lighting += specular + diffuse;
 }

 FragColor = vec4( lighting, 1.0);

 }
 */

vec2 hammersley(uint i, uint N)
{
  return vec2(
    float(i) / float(N),
    float(bitfieldReverse(i)) * 2.3283064365386963e-10
  );
}

// GGX Geometric Shadowing using Smith's Method
float GGXSmith(float NDotL, float NDotV)
{

    float a = roughnessValue;
    float k = ((a + 1) * (a +1))/8;
    float G_V = NDotV / (NDotV * (1-k) +k);
    float G_L = NDotL / (NDotL * (1-k) +k);

    /*float G_V = NDotV + sqrt(aSquared + (1-aSquared) * (NDotV * NDotV));
     float G_L = NDotL + sqrt(aSquared + (1-aSquared) * (NDotL * NDotL));
     G_V = 2*NDotV/G_V;
     G_L = 2*NDotL/G_L;*/
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


vec3 ImportanceSampleGGX( vec2 Xi, float Roughness, vec3 N )
{
    float a = Roughness * Roughness;
    float Phi = 2 * 3.14159 * Xi.x;
    float CosTheta = sqrt( (1 - Xi.y) / ( 1 + (a*a - 1) * Xi.y ) );
    float SinTheta = sqrt( 1 - CosTheta * CosTheta );
    vec3 H;
    H.x = SinTheta * cos( Phi );
    H.y = SinTheta * sin( Phi );
    H.z = CosTheta;

    vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
    vec3 TangentX = normalize( cross( UpVector, N ) );
    vec3 TangentY = cross( N, TangentX );
    // Tangent to world space
    return TangentX * H.x + TangentY * H.y + N * H.z;
}

vec2 panoramaCon(vec3 ray)
{
    // Note: Two arguments of atan is atan2
    return vec2(0.5 + 0.5*atan(ray.x, -ray.z)/3.14159, 1.0 - acos(ray.y)/3.14159);
}



vec3 PreFilterEnvMap(float Roughness, vec3 R)
{
  vec3 N = R;
  vec3 V = R;

  vec3 PrefilteredColor = vec3(0,0,0);
  float TotalWeight = 0;
  uint NumSamples = 16;
  for(uint i = 0; i < NumSamples; i++)
  {
    vec2 Xi = hammersley(i,NumSamples);
    vec3 H = ImportanceSampleGGX(Xi,Roughness,N);
    vec3 L = 2 * dot( V, H ) * H - V;

    float NdotL = clamp(dot(N,L),0.0,1.0);

    if(NdotL > 0)
    {
      PrefilteredColor += texture(Panorama,L).rgb * NdotL;
      TotalWeight += NdotL;
    }
  }
  return PrefilteredColor/TotalWeight;
}

vec2 IntegrateBRDF(float Roughness, float NdotV, vec3 N)
{
  vec3 V;
  V.x = sqrt(1.0 - NdotV * NdotV);
  V.y = 0;
  V.z = NdotV;

  float A = 0;
  float B = 0;
  uint NumSamples = 16;

  for(uint i = 0; i < NumSamples; i++)
  {
    vec2 Xi = hammersley(i,NumSamples);
    vec3 H = ImportanceSampleGGX(Xi,Roughness,N);
    vec3 L =  2 * dot( V, H ) * H - V;

    float NdotL = clamp(L.z,0.0,1.0);
    float NdotH = clamp(H.z,0.1,1.0);
    float VdotH = clamp(dot(V,H),0.0,1.0);

    if(NdotL > 0)
    {
      float G = GGXSmith(NdotV,NdotL);

      float G_Vis = G * VdotH / 1;

      A +=  FresnelSchlick(NdotH)* G_Vis;
      B += FresnelSchlick(NdotH) * G_Vis;
    }
  }
  return vec2(A,B) / NumSamples;
}

vec3 ApproximateSpecIBL(vec3 SpecularColor, float Roughness, vec3 N, vec3 V)
{

  float NdotV = clamp(dot(N,V),0.0,1.0);
  vec3 R =  2 * dot( V, N ) * N - V;

  vec3 PrefilteredColor = PreFilterEnvMap(Roughness, R);
  vec2 EnvBRDF = IntegrateBRDF(Roughness, NdotV,N);

  return PrefilteredColor * (SpecularColor * EnvBRDF.x + EnvBRDF.y);
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

            // Lambertian Diffuse Reflection
            diffuse = max(dot(Normal, lightDirection), 0.0) * Diffuse * light.Color * light.intensity;


            //float Rs = (FresnelSchlick(VdotH) * GGXSmith(NdotL,NdotV,mSquared) * GGXNDF(NdotH,mSquared)) / (NdotV * NdotL * 3.14);
            //  spec = Specular * (light.Color * NdotL * (k + Rs * (1.0 - k)));
        }
        // attenuation of light
        // fall off of light
        float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normalize(Normal));

        lighting += texture(Panorama,R,6).rgb;//(1 * ApproximateSpecIBL(vec3(1,1,1),roughnessValue, normal,viewPos)) + diffuse;
        lighting *= attenuation;
    }


    FragColor = vec4(lighting, 1.0);
}
