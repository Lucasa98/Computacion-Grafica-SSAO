#version 150 core

in vec3 vertPosition;
in vec3 vertColor;

uniform mat4 viewmatrix;
uniform mat4 projectionmatrix;

out vec3 color;
// out vec4 gl_Position		No se declara porque es obligatoria asi que no tiene sentido

void main() {
	color = vertColor;
	gl_Position = projectionmatrix*viewmatrix*vec4(vertPosition, 1.0);	// 1.0 porque es un punto, 0.0 si fuera vector
}
