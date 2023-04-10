#include <glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include "../structs.hpp"

const unsigned int DEFAULT_WIDTH = 256;
const unsigned int DEFAULT_HEIGHT = 192;
const unsigned int BG_TILE_SIZE = 16;

/*
const char* VS_SOURCE = "#version 460 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "}\0";
/*
*/

/*
const char* FS_SOURCE = "#version 460 core\n"
	"out vec4 FragColor;\n"

	"void main()\n"
	"{\n"
    "	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";
/*
*/

const char* VS_SOURCE = "#version 460 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec2 aTexCoord;\n"
	"out vec2 TexCoord;"

	"void main()\n"
	"{\n"
    "	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
	"	TexCoord = aTexCoord;\n"
	"}\0";



const char* FS_SOURCE = "#version 460 core\n"
	"out vec4 FragColor;\n\n"
	"in vec2 TexCoord;\n\n"
	"uniform sampler2D screenTexture;\n\n"
	"void main()\n"
	"{\n"
	"	FragColor = texture(screenTexture, TexCoord);\n"
	"}\0";
	

	
void updateTextures(GameState* state);
void initializeShaders(GameState* state);
void initializeBuffers(GameState* state);
void drawMenu(GameState* state);
unsigned char* getSprite(unsigned char* imageData, int spriteNum, int tileSize, int numChannels);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* initializeWindow(GameState* state) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Your RPG Name", NULL, NULL);
	if (window == NULL) {
		std::cout << "glfwCreateWindow failure";
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "gladLoadGLLoader failure" << std::endl;
		glfwTerminate();
		return NULL;
	}
	
	initializeShaders(state);
	initializeBuffers(state);
	
	
	
	

	return window;
}

void renderFrame(GameState* state) {
	updateTextures(state);
	glUseProgram(state->renderData->shaderProgram);
	glBindTexture(GL_TEXTURE_2D, state->renderData->texture);
	glBindVertexArray(state->renderData->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}

void updateTextures(GameState* state) {
	
	if (!state->mapLoaded) {
		drawMenu(state);
		state->mapLoaded = true;
	}
	
}


void initializeShaders(GameState* state) {
	
	
	/*	OpenGL has two mandatory shaders and three optional ones which control
		the rendering pipeline. The two mandatory ones, which are the vertex
		shader (1st in the pipeline) and fragment shader (4th in the pipeline)
		have their code included in constants at the top of the file. These two
		programs are compiled into a shader program which OpenGL uses whenever
		it renders graphics.
	*/
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VS_SOURCE, NULL);
	glCompileShader(vertexShader);
	
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FS_SOURCE, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	
	state->renderData->shaderProgram = glCreateProgram();
	glAttachShader(state->renderData->shaderProgram, vertexShader);
	glAttachShader(state->renderData->shaderProgram, fragmentShader);
	glLinkProgram(state->renderData->shaderProgram);
	
	glGetProgramiv(state->renderData->shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(state->renderData->shaderProgram, 512, NULL, infoLog);
		printf("Error");
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void initializeBuffers(GameState* state) {
	//Vertex Data
	float vertices[] = {
		//Points        //Textures 
		-1.0f, -1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f,   0.0f, 1.0f,
		 1.0f, -1.0f,   1.0f, 0.0f,
		 1.0f,  1.0f,   1.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		0, 2, 3
	};
	
	//TODO: Add comments
	glGenVertexArrays(1, &(state->renderData->VAO));
	glBindVertexArray(state->renderData->VAO);
	
	glGenBuffers(1, &(state->renderData->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, state->renderData->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glGenBuffers(1, &(state->renderData->EBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->renderData->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glGenTextures(1, &(state->renderData->texture));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, state->renderData->texture);

	
	
}

void drawMenu(GameState* state) {
	/*
	cimg_library::CImg<unsigned char> image("assets/BasicSprite.png");
	
	printf("%d\n", image.size());
	image.print("asset", true);
	*/
	
	int width, height, numChannels;
	unsigned char* data = stbi_load("../assets/BasicSprite.png", &width, &height, &numChannels, 0);
	
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 192, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)0);
		int i, j;
		for (i=0; i < DEFAULT_HEIGHT; i+=BG_TILE_SIZE) {
			for (j=0; j < DEFAULT_WIDTH; j+=BG_TILE_SIZE) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, j, i, BG_TILE_SIZE, BG_TILE_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, getSprite(data, ((i+j)/16)%8, BG_TILE_SIZE, numChannels));
			}
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		printf("Error");
	}
	
}

unsigned char* getSprite(unsigned char* imageData, int spriteNum, int tileSize, int numChannels) {
	return imageData + numChannels*tileSize*tileSize*spriteNum;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}