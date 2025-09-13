#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "UI.h"
#include <string>

UI::UI() {
	quad = NULL;
}

UI::~UI() {
	if (quad != NULL)
		delete quad;
	for (int i = 0; i < 4; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}

void UI::init(const int& level, const int& lives, const bool& godMode) {
	
	game_ui = level;
	hp = lives;
	god = godMode;

	power = 0;


	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(36.f, 36.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };


	initShaders();

	currentTime = 0.0f;
	timeAccumulatorCoin = 0.0f;
	timeAccumulatorTimer = 0.0f;
	timerInverse = 90;
	showInsertCoin = false;

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texs[0].loadFromFile("images/hp.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);
	texs[1].loadFromFile("images/Stop.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].setMagFilter(GL_NEAREST);
	texs[2].loadFromFile("images/DoubleHook.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);
	texs[3].loadFromFile("images/Burst.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].setMagFilter(GL_NEAREST);



	projection = glm::ortho(0.f, float(CAMERA_WIDTH), float(CAMERA_HEIGHT), 0.f);

	if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void UI::update(int deltaTime, int& lives, bool& godMode, int totalPoints) {
	currentTime += deltaTime;
	timeAccumulatorCoin += deltaTime;
	timeAccumulatorTimer += deltaTime;

	hp = lives;
	god = godMode;
	points = totalPoints;
}

int UI::getCountDown() {
	return time;
}

void UI::setPower(int poder) {
	power = poder;
}

void UI::usePower() {
	power = 0;
}

void UI::render() {

	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	if (power != 0) {
		if (power == 1) {
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(200.f, CAMERA_HEIGHT - 120.f, 0.f));  // Cambia la altura según sea necesario
			modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
			modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			texQuad[3]->render(texs[1]);
		}
		if (power == 2) {
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(200.f, CAMERA_HEIGHT - 120.f, 0.f));  // Cambia la altura según sea necesario
			modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
			modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			texQuad[3]->render(texs[2]);
		}
		if (power == 3) {
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(200.f, CAMERA_HEIGHT - 120.f, 0.f));  // Cambia la altura según sea necesario
			modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
			modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			texQuad[3]->render(texs[3]);
		}
	}

	if (hp == 3) {
		// Renderizar texQuad[0] en la parte inferior
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(50.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura según sea necesario
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[0]);

		// Renderizar texQuad[1] en la parte inferior
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(90.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura según sea necesario
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[1]->render(texs[0]);

		// Renderizar texQuad[2] en la parte inferior
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(130.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura según sea necesario
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[2]->render(texs[0]);
	}

	if (hp == 2) {
		// Renderizar texQuad[0] en la parte inferior
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(50.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura según sea necesario
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[0]);

		// Renderizar texQuad[1] en la parte inferior
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(90.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura según sea necesario
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[1]->render(texs[0]);
	}
	if (hp == 1) {
		// Renderizar texQuad[0] en la parte inferior
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(50.f, CAMERA_HEIGHT - 40.f, 0.f));  // Cambia la altura según sea necesario
		modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[0]);
	}

	if (timeAccumulatorTimer >= 1000) {
		int countdown = 100 - (currentTime / 1000);
		countdown = std::max(0, countdown);
		time = countdown;

		if (god && countdown == 0) {
			timeScreen = "TIME: 000";
		}
		else {
			timeScreen = "TIME: ";
			if (countdown < 10) {
				timeScreen += "00";
			}
			else if (countdown < 100) {
				timeScreen += "0";
			}
			timeScreen += std::to_string(countdown);
		}

		timeAccumulatorTimer -= 1000;
	}

	text.render(timeScreen, glm::vec2(CAMERA_WIDTH - 270, 65), 50, glm::vec4(1, 1, 1, 1));


	if (game_ui == 1){
		text.render("MT.FUJI", glm::vec2(CAMERA_WIDTH / 2 - 65, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
		text.render("STAGE 1", glm::vec2(CAMERA_WIDTH / 2 - 55, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
	}
	if (game_ui == 2) {
		text.render("LONDON", glm::vec2(CAMERA_WIDTH / 2 - 70, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
		text.render("STAGE 2", glm::vec2(CAMERA_WIDTH / 2 - 55, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
	}
	if (game_ui == 3) {
		text.render("BARCELONA", glm::vec2(CAMERA_WIDTH / 2 - 100, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));
		text.render("STAGE 3", glm::vec2(CAMERA_WIDTH / 2 - 55, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
	}

	text.render("PLAYER-1", glm::vec2(50, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));


	text.render(std::to_string(points), glm::vec2(150, CAMERA_HEIGHT - 60), 32, glm::vec4(1, 1, 1, 1));

	if (god) text.render("GODMODE", glm::vec2(170, CAMERA_HEIGHT - 10), 32, glm::vec4(1, 1, 1, 1));


	text.render("PLAYER-2", glm::vec2(CAMERA_WIDTH - 300, CAMERA_HEIGHT - 90), 32, glm::vec4(1, 1, 1, 1));

	if (timeAccumulatorCoin >= 500.0f) {
		showInsertCoin = !showInsertCoin;
		timeAccumulatorCoin = 0.0f;
	}
	if (showInsertCoin) text.render("INSERT COIN", glm::vec2(CAMERA_WIDTH - 300, CAMERA_HEIGHT - 40), 26, glm::vec4(1, 1, 1, 1));
}

void UI::initShaders()
{

	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
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
}