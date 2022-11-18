#ifndef LOAD_SHADERS_H
#define LOAD_SHADERS_H

#include <GL/glew.h>
#include <string>

GLuint load_shaders(const std::string &vertex_path, const std::string &fragment_path);

#endif

