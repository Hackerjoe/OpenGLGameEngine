#version 410 core
uniform samplerCube cubeMap;
uniform int mipLevel;
uniform int cubeSize;
uniform	float Roughness;
uniform int face;

in vec2 texcoords;

out vec4 color;

/** Defines **/
#define M_PI 3.1415926535897932384626433832795
#define M_PI2 M_PI * 2
#define PI    3.14159265359
#define INVPI 0.31830988618
#define EPS   1e-5

float saturate (float x) {
	return clamp(x, 0.0, 1.0);
}

vec3 saturate (vec3 v) {
	return clamp(v, 0.0, 1.0);
}

float radicalInverse_VdC(uint bits) {
	 bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
 }

 // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
 vec2 Hammersley(uint i, uint N) {
     return vec2(float(i)/float(N), radicalInverse_VdC(i));
 }

vec3 ImportanceSampleGGX( vec2 E, float Roughness, vec3 N ) {
	float m = Roughness * Roughness;

	float Phi = 2 * PI * E.x;
	float CosTheta = sqrt( (1 - E.y) / ( 1 + (m*m - 1) * E.y ) );
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );

	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;

	vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector, N ) );
	vec3 TangentY = cross( N, TangentX );
	// tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

 // Ignacio Castano via http://the-witness.net/news/2012/02/seamless-cube-map-filtering/
vec3 fix_cube_lookup_for_lod(vec3 v, float cube_size, float lod) {
	float M = max(max(abs(v.x), abs(v.y)), abs(v.z));
	float scale = 1 - exp2(lod) / cube_size;
	if (abs(v.x) != M) v.x *= scale;
	if (abs(v.y) != M) v.y *= scale;
	if (abs(v.z) != M) v.z *= scale;
	return v;
}

vec3 UvAndIndexToBoxCoord(vec2 uv, int face){
	vec3 n = vec3(0, 0, 0);
	vec3 t = vec3(0, 0, 0);

	// posx (red)
	if (face == 0) {
		n = vec3(1, 0, 0);
		t = vec3(0, 1, 0);
	}
	// negx (cyan)
	else if (face == 1) {
		n = vec3(-1, 0, 0);
		t = vec3(0, 1, 0);
	}
	// posy (green)
	else if (face == 2) {
		n = vec3(0, -1, 0);
		t = vec3(0, 0, -1);
	}
	// negy (magenta)
	else if (face == 3) {
		n = vec3(0, 1, 0);
		t = vec3(0, 0, 1);
	}
	// posz (blue)
	else if (face == 4) {
		n = vec3(0, 0, -1);
		t = vec3(0, 1, 0);
	}
	// negz (yellow)
	else {
		n = vec3(0, 0, 1);
		t = vec3(0, 1, 0);
	}

	vec3 x = cross(n, t);

	uv = uv * 2 - 1;

	n = n + t * uv.y + x * uv.x;
	n.y *= -1;
	n.z *= -1;
	return n;
}

void main(){

	vec3 V = UvAndIndexToBoxCoord(texcoords, face);
	vec3 N = fix_cube_lookup_for_lod(V, cubeSize, mipLevel);

	vec4 totalRadiance = vec4(0, 0, 0, 0);
	uint maxSamples = 1024u;
	for(uint i = 0u; i < maxSamples; i++){
		vec2 Xi = Hammersley(i, maxSamples);
		vec3 H = ImportanceSampleGGX(Xi, Roughness, N);
		vec3 L = 2 * dot(N, H) * H - N;
    float nDotL = saturate(dot(L, N));

    	if (nDotL > 0){
		      vec4 pointRadiance = texture(cubeMap, L);
		      totalRadiance.rgb += pointRadiance.rgb * nDotL;
		      totalRadiance.w += nDotL;
		  }
	}
	color = vec4(totalRadiance.rgb / totalRadiance.w, 1);


}
