#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;     // toma una textura extra para el ssao

struct Light {
    vec3 Position;
    vec3 Color;
};
uniform Light light;
uniform vec3 viewPos;

void main()
{             
    // obtener posicion, normal y color del gBuffer y oclusion del ssaoBuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;
    
    // calcular iluminacion como siempre
    vec3 lighting  = Diffuse * 0.1 * AmbientOcclusion; // ambiente hardcodeada
    vec3 viewDir  = normalize(viewPos - FragPos);
    
    // diffuse
    vec3 lightDir = normalize(light.Position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;
    
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 40);
    vec3 specular = light.Color * spec * Specular;
    lighting += diffuse + specular;
    FragColor = vec4(lighting, 1.0);
}