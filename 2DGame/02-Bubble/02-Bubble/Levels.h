#ifndef _LEVELS_INCLUDE
#define _LEVELS_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"


class Levels {
public:
	Levels();
	~Levels();

	void init(const int& level);
	void render();
	void update(int deltaTime);

private:
	void initShaders();

private:
	float currentTime;
	TexturedQuad* background;
	glm::mat4 projection;
	Texture scene, level1, level2, level3;

	ShaderProgram texProgram;
};

#endif // _LEVELS_INCLUDE