#include "helpers.h"

float readInt(const std::string &s, int &i) {
	char *p = const_cast<char*>(s.c_str())+i;
	int r = std::strtol(p,&p,10);
	i = p-s.c_str();
	return r;
}

float readFloat(const std::string &s, int &i) {
	char *p = const_cast<char*>(s.c_str())+i;
	float r = std::strtof(p,&p);
	i = p-s.c_str();
	return r;
}

float readFloat(const std::string &s, const int &i) {
	int j = i; return readFloat(s,j);
}

glm::vec3 readVec3(const std::string &s, int i) {
	glm::vec3 v;
	v.x = readFloat(s,i); ++i;
	v.y = readFloat(s,i); ++i;
	v.z = readFloat(s,i);
	return v;
}

glm::vec2 readVec2(const std::string &s, int i) {
	glm::vec2 v;
	v.x = readFloat(s,i); ++i;
	v.y = readFloat(s,i);
	return v;
}

std::string extractFolder(const std::string &filename) {
	int i = static_cast<int>(filename.size())-1;
	std::string path;
	while(i>0 and filename[i]!='\\' and filename[i]!='/' and filename[i]!=':') 
		--i;
	return filename.substr(0,i+1);
}

bool startsWith(const std::string str, const char *con) {
	int i=0, l=str.size();
	for(;con[i] && i<l;++i)
		if (con[i]!=str[i]) return false;
	return con[i]=='\0';
}
