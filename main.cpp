#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "load_shaders.h"
#include "Geometry.h"

// view matrix
glm::mat4 mv = glm::lookAt( glm::vec3(2.f,3.f,5.f), glm::vec3(0.f,0.f,0.f), glm::vec3{0.f,1.f,0.f} );

// projection matrix
glm::mat4 mp = glm::perspective( glm::radians(45.f), 800.f/600.f, 0.1f, 1000.f );

// cerrar ventana con ESCAPE callback
void keyboard_cb(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key==GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window,GL_TRUE);
}

// mouse click callback
void mouse_cb(GLFWwindow *window, int button, int action, int mods){
	// al soltar el boton izquierdo
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		// obtener posicion del mouse (pixel de la ventana)
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
	}
}

int main() {
	
	/// inicializar glfw
	glfwInit();
	
	// definir el tipo de contexto opengl que vamos a necesitar (version, ponele)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	
	// crear y activar la ventana
	GLFWwindow *window = glfwCreateWindow(800,600,"Ejemplo GLFW3",nullptr,nullptr);		//Crear ventana
	glfwMakeContextCurrent(window);
	
	// registrar callbacks
	glfwSetKeyCallback(window,keyboard_cb);		//cerrar cuando apretas Escape
	glfwSetMouseButtonCallback(window,mouse_cb);
	
	// inicializar glew
	glewExperimental = GL_TRUE;
	glewInit();
	
	// cargar shaders
	GLuint shader_program = load_shaders("shader.vert","shader.frag");
	glUseProgram(shader_program);
	
	Geometry geo("cube.obj");
	
	
	// bucle de eventos
	while(not glfwWindowShouldClose(window)) {
		/// renderizar =====
		
		// limpiar el color buffer y z-buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		geo.draw();
		
		/// ================
		
		// swapear
		glfwSwapBuffers(window);
		
		// que glfw detecte y procese los eventos pendientes
		glfwPollEvents();
	}
	
	glfwTerminate();
	
}
