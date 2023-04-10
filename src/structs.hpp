#ifndef STRUCTS_H
#define STRUCTS_H
struct RenderData{
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int shaderProgram;
	unsigned int texture;
};

struct GameState {
	RenderData* renderData;
	bool mapLoaded = false;
};

#endif