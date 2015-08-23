#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
};

uniform vec3 viewPos;
uniform Light light;

vec2 CalcTexCoord()
{
   vec2 screensize = vec2(1920,1080) ;
   return gl_FragCoord.xy/(screensize);
}

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
      float distance = length(light.Position - FragPos);
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
      lighting += diffuse + specular;
    }




    FragColor = vec4(lighting, 1.0);
}
