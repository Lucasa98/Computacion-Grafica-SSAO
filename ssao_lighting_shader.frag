#version 330 core
out float FragColor;

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
    // obtener las entradas para calcular AO
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

    // matriz de cambio de base(de tangent-space a view-space)
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 binormal = cross(normal, tangent);
    mat3 TBN = mat3(tangent, binormal, normal);

    // calculamos un factor de oclusion por cada fragmento
    float occlusion = 0.0;
    for(int i = 0; i < samplesNum; ++i)
    {
        // get sample position
        vec3 samplePos = TBN * samples[i]; // de tangent-space a view-space
        samplePos = fragPos + samplePos * radius; 
        
        // proyectamos la muestra para obtener su posicion en la pantalla (y en la textura)
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // view-space -> clip-space
        offset.xyz /= offset.w; // division perspectiva
        offset.xyz = offset.xyz * 0.5 + 0.5; // -> screen-space [0.0, 1.0]
        
        // profundidad del fragmento sobre el que se proyecta
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
        
        // interpolacion suave para eliminar ruido (en gran parte).
        float rangeCheck;
        if (ssaoSmooth){
            rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        }else{
            rangeCheck = radius / abs(fragPos.z - sampleDepth);
        }

        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck * intensity;           
    }
    occlusion = 1.0 - (occlusion / samplesNum);
    
    FragColor = occlusion;
}