#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "Text.h"

#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 624

class Menu {
public:
	Menu();
	~Menu();

	void init(const int& idScene);
	void render();
	void update(int deltaTime);
	void setScore(int score);

private:
	void initShaders();

private:
	float currentTime, timeAccumulatorCoin;
	int totalPoints;
	bool showInsertCoin;
	int type;
	TexturedQuad *background;
	glm::mat4 projection;
	Texture scene, menu, instructions;

	ShaderProgram texProgram;
	Text text;
};

#endif // _MENU_INCLUDE