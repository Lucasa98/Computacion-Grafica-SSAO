#include <fstream>
#include <iostream>
#include "load_shaders.h"

std::string read_file(const std::string &path) {
	std::string content;
	std::ifstream file(path,std::ios::binary|std::ios::ate);
	if (not file.is_open()) {
		std::cerr << "No se pudo leer el archivo " << path << std::endl;
		exit(1);
	}
	content.resize(file.tellg());
	file.seekg(0);
	file.read(&(content[0]),content.size());
	return content;
}

GLuint compile_shader(const std::string &path, GLenum type) {
	// crear un shader en blanco
	GLuint handler = glCreateShader(type);
	// cargar y asignar el codigo fuente
	std::string src = read_file(path);
	const char *src_ptr = src.c_str();
	int src_len = src.size();
	glShaderSource(handler,1,&src_ptr,&src_len);
	// compilar y verificar si hay errores
	glCompileShader(handler);
	GLint status;
	glGetShaderiv(handler,GL_COMPILE_STATUS, &status);
	if (status!=GL_TRUE) { // si no compilo, mostrar errores
		char buffer[4096];
		glGetShaderInfoLog(handler,4096,nullptr,buffer);
		std::cerr << "Errores compilando " << path << std::endl;
		std::cerr << buffer << std::endl;
		exit(1);
	}
	return handler;
}

GLuint load_shaders(const std::string &vertex_path, const std::string &fragment_path) {
	// crear programa
	GLuint program_handler = glCreateProgram();
	// cargar y compilar cada shader
	GLuint vertex_handler = compile_shader(vertex_path,GL_VERTEX_SHADER);
	GLuint fragment_handler = compile_shader(fragment_path,GL_FRAGMENT_SHADER);
	// enlazar el programa final
	glAttachShader(program_handler,vertex_handler);
	glAttachShader(program_handler,fragment_handler);
	glLinkProgram(program_handler);
	return program_handler;
}
