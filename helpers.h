#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>

float readInt(const std::string &s, int &i);

float readFloat(const std::string &s, int &i);

float readFloat(const std::string &s, const int &i);

glm::vec3 readVec3(const std::string &s, int i);

glm::vec2 readVec2(const std::string &s, int i);

std::string extractFolder(const std::string &filename);

bool startsWith(const std::string str, const char *con);
