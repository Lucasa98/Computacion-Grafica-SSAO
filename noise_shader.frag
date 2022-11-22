#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// parametros
uniform int samplesNum = 64;
uniform float radius = 0.5;
uniform float bias = 0.025;
uniform float intensity = 1.0;
uniform bool ssaoSmooth = true;

// textura de ruido
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); 

uniform mat4 projection;

void main()
{
    // obtener noise vector
    vec3 noiseVec = texture(texNoise, TexCoords * noiseScale).xyz;
    
    FragColor = vec4(noiseVec, 1.0);
}