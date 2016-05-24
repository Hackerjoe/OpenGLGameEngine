#version 410 core
in vec4 postions;
in vec3 views;
out vec4 color;

uniform samplerCube skybox;

float radicalInverse_VdC(uint bits)
{
  bits = (bits << 16u) | (bits >> 16u);
  bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
  bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
  bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
  bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

  return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
vec2 Hammersley(uint i, uint n)
{
  return vec2(i/n, radicalInverse_VdC(i));
}

vec3 TangentToWorld( vec3 Vec, vec3 TangentZ )
{
	vec3 UpVector = abs(TangentZ.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector, TangentZ ) );
	vec3 TangentY = cross( TangentZ, TangentX );
	return TangentX * Vec.x + TangentY * Vec.y + TangentZ * Vec.z;
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

vec3 Spec(float Roughness, vec3 R)
{
	vec3 N = R;
	vec3 V = R;
	float TotalWeight = 0;
	vec3 PrefilteredColor = vec3(0,0,0);
	const uint NumSamples = 1024;
	for (uint i = 0; i < NumSamples; i++)
	{
	 	vec2 Xi = Hammersley(i,NumSamples);
	 	vec3 H = ImportanceSampleGGX(Xi, Roughness,N).xyz;
	 	vec3 L = 2 * dot(V, H) * H - V;
	 	float NoL = clamp(dot(N,L),0,1);

	 	if (NoL > 0)
	 	{
				PrefilteredColor += texture(skybox,L).xyz;
				TotalWeight += NoL;
		}
	}

	return PrefilteredColor / TotalWeight;
}


void main()
{
	vec3 v = normalize(views);
	vec3 spewc = Spec(0.0,v).xyz;
	color = vec4(spewc,1.0);
}
