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

/*
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


vec2 hammersley(uint originalSample)
{

    uint revertSample;

    // Revert bits by swapping blockwise. Lower bits are moved up and higher bits down.
    revertSample = (originalSample << 16u) | (originalSample >> 16u);
    revertSample = ((revertSample & 0x00ff00ffu) << 8u) | ((revertSample & 0xff00ff00u) >> 8u);
    revertSample = ((revertSample & 0x0f0f0f0fu) << 4u) | ((revertSample & 0xf0f0f0f0u) >> 4u);
    revertSample = ((revertSample & 0x33333333u) << 2u) | ((revertSample & 0xccccccccu) >> 2u);
    revertSample = ((revertSample & 0x55555555u) << 1u) | ((revertSample & 0xaaaaaaaau) >> 1u);


    // Shift back, as only m bits are used.
    revertSample = revertSample >> (32 - 9);
    //std::cout << revertSample << std::endl;
    vec2 returnVec;
    // S
    float PowF = 1.0f / (pow(2.0f,9.0f) - 1.0f);


    returnVec.x = float(revertSample) * PowF;
    returnVec.y = float (originalSample) * PowF;

    return returnVec;
}


vec3 TangentToWorld( vec3 Vec, vec3 TangentZ )
{
	vec3 UpVector = abs(TangentZ.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector, TangentZ ) );
	vec3 TangentY = cross( TangentZ, TangentX );
	return TangentX * Vec.x + TangentY * Vec.y + TangentZ * Vec.z;
}



// GGX Geometric Shadowing using Smith's Method
float GGXSmith(float NDotL, float NDotV, float aSquared)
{
    float a = sqrt(aSquared);
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


vec4 ImportanceSampleGGX( vec2 E, float Roughness )
{
	float m = Roughness * Roughness;
	float m2 = m * m;

	float Phi = 2 * 3.14159 * E.x;
	float CosTheta = sqrt( (1 - E.y) / ( 1 + (m2 - 1) * E.y ) );
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );

	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;

	float d = ( CosTheta * m2 - CosTheta ) * CosTheta + 1;
	float D = m2 / ( 3.14159*d*d );
	float PDF = D * CosTheta;

	return vec4( H, PDF );
}

vec2 panoramaCon(vec3 ray)
{
    // Note: Two arguments of atan is atan2
    return vec2(0.5 + 0.5*atan(ray.x, -ray.z)/3.14159, 1.0 - acos(ray.y)/3.14159);
}


vec3 SpecularIBL(vec3 SpecularColor, float Roughness, vec3 N, vec3 V)
{
    vec3 SpecularLighting = vec3(0,0,0);

    const uint NumSamples = 512;

    float NV = max(dot(N, V), 0.0);


    for (uint i = 0; i < NumSamples; i++)
    {
        vec2 Xi = hammersley(i);

        vec3 H = TangentToWorld(ImportanceSampleGGX(Xi,Roughness).xyz,N);

        vec3 L = 2 * dot(V,H) * H - V;

        float NH = clamp(dot(N,H),0,1);
        float VH = clamp(dot(V,H),0,1);
        float NL =clamp(dot(N,L),0,1);

        if (NL > 0.0)
        {
            vec3 SampleColor = texture(Panorama,L).rgb;

            float G = GGXSmith(NL,NV,Roughness*Roughness);

            float F = FresnelSchlick(VH);

            F = clamp(F,0,1);

            SpecularLighting += (SampleColor * F * G * VH) / (NH * NV);


        }

    }

    return SpecularLighting / NumSamples;
}

vec3 cosineWeightedSampling(vec2 e)
{
	float x = sqrt(1.0 - e.x) * cos(2.0*3.14159*e.y);
	float y = sqrt(1.0 - e.x) * sin(2.0*3.14159*e.y);
	float z = sqrt(e.x);

	return vec3(x, y, z);
}

vec3 brdfLambert(vec3 Color, vec3 N)
{

      const uint NumSamples = 512;
      vec3 DiffuseLighting = vec3(0,0,0);
      for (uint i = 0; i < NumSamples; i++)
      {
         vec2 Xi = hammersley(i);
	       vec3 LtangentSpace = TangentToWorld(cosineWeightedSampling(Xi),N);
         DiffuseLighting += Color * texture(Panorama,LtangentSpace).rgb;
      }

	     return DiffuseLighting / NumSamples;
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
            //  vec3 halfVector = normalize(lightDirection + eyeDir);
            //  float NdotH = max(dot(normal, halfVector), 0.0);
            //  float NdotV = max(dot(normal, eyeDir), 0.0);
            //  float VdotH = max(dot(eyeDir, halfVector), 0.0);
            //  float mSquared = roughnessValue * roughnessValue;

            //float Rs = (FresnelSchlick(VdotH) * GGXSmith(NdotL,NdotV,mSquared) * GGXNDF(NdotH,mSquared)) / (NdotV * NdotL * 3.14);
            //  spec = Specular * (light.Color * NdotL * (k + Rs * (1.0 - k)));


        }
        // attenuation of light
        // fall off of light
        float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
        vec3 I = normalize(FragPos - viewPos);
        vec3 R = reflect(I, normalize(Normal));

        lighting += textureLod(Panorama,R,2.0).rgb;
        //lighting += texture(Panorama, FragPos).rgb;//(Specular * SpecularIBL(light.Color, roughnessValue, normal, viewPos) )+ diffuse; //+ texture(Panorama,CalcTexCoord()).rgb;
        lighting *= attenuation;
    }


    FragColor = vec4(lighting, 1.0);
}
