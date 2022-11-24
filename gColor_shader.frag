#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light {
    vec3 Position;
    vec3 Color;
};
uniform Light light;

void main()
{             
    // Obtener el color
    vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;

    FragColor = vec4(Diffuse, 1.0);
}