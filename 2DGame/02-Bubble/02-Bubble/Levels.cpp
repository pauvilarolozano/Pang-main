#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Levels.h"
#include "Game.h"

Levels::Levels() {
	//currentTime = 0.0f;

}

Levels::~Levels() {
	if (background != NULL)
		delete background;
}

void Levels::init(const int& level) {

	currentTime = 0.0f;

	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(1152.f, 624.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);


	if (level == 1) {
		scene.loadFromFile("images/backgroundLevel01.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	if (level == 2) {
		scene.loadFromFile("images/backgroundLevel02.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	if (level == 3) {
		scene.loadFromFile("images/backgroundLevel03.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}

	scene.setMinFilter(GL_NEAREST);
	scene.setMagFilter(GL_NEAREST);
}

void Levels::update(int deltaTime) {
	currentTime += deltaTime;

}

void Levels::render() {

	background->render(scene);
}


void Levels::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
