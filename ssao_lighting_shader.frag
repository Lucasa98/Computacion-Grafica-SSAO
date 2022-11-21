#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;    // gBuffer
uniform sampler2D gNormal;      // gBuffer
uniform sampler2D texNoise;     // textura de 4x4 que construimos para rotaciones aleatorias

uniform vec3 samples[64];

// SSAO parameters
int samplesNum = 64;
float radius = 0.5;
float bias = 0.025;
float ssaoIntensity = 1.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); 

uniform mat4 projection;

void main()
{
    // entrada para calcular SSAO
    vec3 fragPos = texture(gPosition, TexCoords).xyz;                           // posicion del fragmento
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);                   // normal del fragmento
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);  // vector de rotacion aleatorio

    // matriz de tangent-space a view-space (no entiendo como funciona)
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    // iteramos sobre las muestras (samples) y calculamos el factor de oclusion
    float occlusion = 0.0;
    for(int i = 0; i < samplesNum; ++i){
        // get sample position
        vec3 samplePos = TBN * samples[i];          // samplePos en realidad es un vector desde el fragmento a la muestra
        samplePos = fragPos + samplePos * radius;   // obtenemos la posicion posta en view-space
        
        // proyectar la posicion de la muestra (para tener la posicion en screen-space)
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;               // de view-space a clip-space
        offset.xyz /= offset.w;                     // de clip-space a screen-space
        offset.xyz = offset.xyz * 0.5 + 0.5;        //normalizamos entre 0.0 y 1.0
        
        // Obtener depth de la muestra (z) de la posicion que guardamos en el gBuffer (que en teoria es view-space
        float sampleDepth = texture(gPosition, offset.xy).z;
        
        // hace una interpolacion loca (smooth hermite interpolation) para la distancia a la muestra en relacion con el radio
        //float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth)); voy a usar la distancia y chau
        float range = radius / abs(fragPos.z - sampleDepth);
        // No entiendo por qué la comparación es entre el samplePos.z  el sampleDepth
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * range * ssaoIntensity;  //usa la interpolacion en lugar de la distancia
    }
    occlusion = 1.0 - (occlusion / samplesNum); // occlusion sera 1 al estar completamente rodeado
    
    FragColor = occlusion;
}