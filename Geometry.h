#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "helpers.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Geometry {
private:
	/// vertices, etc
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> tex_coords;
	std::vector<int> triangles;
	
	/// buffers
	GLuint VAO=0, VBO_pos=0, VBO_tcs=0, VBO_norms=0, EBO=0;
	
	int count=0;
	
	void generateNormals();
	
	void readObj(const std::string& path);
	
	template<typename vector>
	static void updateBuffer(GLenum type, GLuint &id, vector &v) {
		if (id==0) {
			glGenBuffers(1, &id);
		}
		glBindBuffer(type, id);
		if (realloc) {
			glBufferData(type, v.size()*sizeof(typename vector::value_type), v.data(), GL_STATIC_DRAW);
		} else
			glBufferSubData(type, 0, v.size()*sizeof(typename vector::value_type), v.data());
	}
public:
	Geometry(const std::string& path);
	void draw() const;
	void freeResources();
	~Geometry();
};
