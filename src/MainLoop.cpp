#include <glad.h>
#include <GLFW/glfw3.h>
#include "Graphics/WindowManager.h"

#include <iostream>

#include "structs.hpp"

int main() {
	GameState* state = (GameState*)malloc(sizeof(GameState));
	GLFWwindow* window = initializeWindow(state);

	while (!glfwWindowShouldClose(window)) {
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderFrame(state);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}