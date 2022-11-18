#include "Geometry.h"

Geometry::Geometry(const std::string& path) {
	
	readObj(path);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// cargar vertices
	updateBuffer(GL_ARRAY_BUFFER, VBO_pos, positions);
	
	// cargar normales
	updateBuffer(GL_ARRAY_BUFFER, VBO_norms, normals);
	
	// textura
	updateBuffer(GL_ARRAY_BUFFER, VBO_tcs, tex_coords);
	
	// triangulos
	updateBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO, triangles);
	
	glBindVertexArray(0);
}

void Geometry::generateNormals(){
	normals.clear();
	normals.resize(positions.size());
	if (triangles.empty()) {
		for(size_t i=0;i<positions.size();i+=3)
			normals[i] = normals[i+1] = normals[i+2] = 
			glm::normalize(
						   glm::cross(
									  (positions[i+2]-positions[i+1]),
									  (positions[i+0]-positions[i+1]) ) );
	} else {
		for(size_t i=0;i<triangles.size();i+=3) {
			auto n = glm::cross( (positions[triangles[i+2]]-positions[triangles[i+1]]),
								(positions[triangles[i+0]]-positions[triangles[i+1]]) );
			normals[triangles[i+0]] += n;
			normals[triangles[i+1]] += n;
			normals[triangles[i+2]] += n;
		}
		for(auto &n : normals) 
			if (glm::dot(n,n)!=0) 
				n = glm::normalize(n);
	}
}

void Geometry::readObj(const std::string& full_path) {
	std::cout << "Reading obj file: " << full_path << "..." << std::endl;
	std::string path = extractFolder(full_path);
	std::ifstream file(full_path);
	if(!file.is_open()) std::cout << "Could not open obj file" << std::endl;
	
	for(std::string line; std::getline(file,line); ) {
		if (line.empty() or line[0]=='#') continue;
		if (startsWith(line,"v ")) {
			positions.push_back(readVec3(line,2));
		} else if (startsWith(line,"vn ")) {
			normals.push_back(readVec3(line,3));
		} else if (startsWith(line,"vt ")) {
			tex_coords.push_back(readVec2(line,3));
		} else if (startsWith(line,"f ")) {
			int is = 2, l = line.size();
			while(is<l) {
				triangles.push_back(readInt(line,is)-1);
				
				// leer indices de las normaes y tcs (ahora no usamos eso)
//				if (line[is]=='/') {
//					if (line[++is]=='/') {
//						e.tcs[in] = -1;
//						e.norms[in] = readInt(line,++is)-1;
//					} else {
//						e.tcs[in] = readInt(line,is)-1;
//						if (line[is]=='/') {
//							e.norms[in] = readInt(line,++is)-1;
//						} else {
//							e.norms[in] = -1;
//						}
//					}
//				} else {
//					e.tcs[in] = -1;
//					e.norms[in] = -1;
//				}
				
				++is;
			}
		}
	}
}

void Geometry::draw() const {
	glBindVertexArray(VAO);
	if (EBO) glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	else glDrawArrays(GL_TRIANGLES, 0,count);
	glBindVertexArray(0);
}

void Geometry::freeResources() {
	if (VAO==0) return;
	if (VBO_pos) glDeleteBuffers(1,&VBO_pos);
	if (VBO_norms) glDeleteBuffers(1,&VBO_norms);
	if (VBO_tcs) glDeleteBuffers(1,&VBO_tcs);
	if (EBO) glDeleteBuffers(1,&EBO);
	glDeleteVertexArrays(1,&VAO);
}
Geometry::~Geometry() {
	freeResources();
}

