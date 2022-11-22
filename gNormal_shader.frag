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
    // retrieve data from gbuffer
    vec3 Normal = texture(gNormal, TexCoords).rgb;

    FragColor = vec4(Normal, 1.0);
}