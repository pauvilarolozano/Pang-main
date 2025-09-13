
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <chrono>
#include <thread>

#include <iostream>
#include <vector>
#include <algorithm>

#include <cstdlib>
#include <ctime>  

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 22
#define INIT_PLAYER_Y_TILES 21

#define TIME_HITBOX 500
#define TIME_TIMER 3000
#define TIME_SCORE 1000

#define TIME_STOP 2000


Scene::Scene()
{
	map = NULL;
	player = NULL;
	levels = NULL;
	ui = NULL;
	menu = NULL;
	//bubble = NULL;

	engine = SoundProgram::instance().getSoundEngine();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (levels != NULL)
		delete levels;
	if (ui != NULL)
		delete ui;
	if (menu != NULL)
		delete menu;
	/*if (bubble != NULL)
		delete bubble;*/
}


void Scene::init(const int& level, const int& lives, bool& godMode, int points){
	
	actualPoints = points;
	totalPoints = points;
	lastBubble = 0;
	contLastBubble = 0;

	equipedWeapon = false;
	mostrarPoints = false;
	mostrarFruita = false;
	posXpoints = 0;
	posYpoints = 0;

	currentTime = 0.0f;
	lvl = level;
	hp = lives;
	god = godMode;

	timerStop = TIME_STOP;
	timerHitbox = TIME_HITBOX;
	timerTime = TIME_TIMER;
	timerScore = TIME_SCORE;
	activeHitbox = true;
	activeTime = false;
	activeScore = false;
	activeStop = false;

	cout << "Level " << level << endl;

	finish = false;

	engine = SoundProgram::instance().getSoundEngine();

	levels = new Levels();
	levels->init(level);

	ui = new UI();
	ui->init(level, hp, godMode);

	while (bubbles.size() > 0) bubbles.pop_back();
	while (fruits.size() > 0) fruits.pop_back();
	while (powers.size() > 0) powers.pop_back();

	initShaders();

	if (level == 1) {
		map = TileMap::createTileMap("levels/pangLevel01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	if (level == 2) {
		map = TileMap::createTileMap("levels/pangLevel02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	if (level == 3) {
		map = TileMap::createTileMap("levels/pangLevel03.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}

	player = new Player();
	player->setTileMap(map);
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));

	if (!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;


	if (level == 1) {

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble1->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		engine->removeAllSoundSources();
		engine->play2D("sounds/MtFuji.mp3");
	}
	if (level == 2) {
		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble1->setPosition(glm::vec2(32 * map->getTileSize(), 2 * map->getTileSize()));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble2->setPosition(glm::vec2(18 * map->getTileSize(), 12 * map->getTileSize()));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		/*		
		Bubble* bubble3 = new Bubble();
		bubble3->init(glm::ivec2(0, 0), texProgram, 2, 0);
		bubble3->setPosition(glm::vec2(20 * map->getTileSize(), 12 * map->getTileSize()));
		bubble3->setTileMap(map);

		bubbles.push_back(bubble3);

		Bubble* bubble4 = new Bubble();
		bubble4->init(glm::ivec2(0, 0), texProgram, 2, 1);
		bubble4->setPosition(glm::vec2(32 * map->getTileSize(), 12 * map->getTileSize()));
		bubble4->setTileMap(map);

		bubbles.push_back(bubble4);
		*/

		engine->removeAllSoundSources();
		engine->play2D("sounds/London.mp3");
	}
	if (level == 3) {
		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble1->setPosition(glm::vec2(4 * map->getTileSize(), 2 * map->getTileSize()));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 1, 0);
		bubble2->setPosition(glm::vec2(18 * map->getTileSize(), 1 * map->getTileSize()));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		engine->removeAllSoundSources();
		engine->play2D("sounds/Barcelona.mp3");
	}


	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

}

void Scene::update(int deltaTime, bool& godMode)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	if (!activeStop) for (auto& bubble: bubbles) bubble->update(deltaTime);
	ui->update(deltaTime, hp, godMode, totalPoints);
	for (auto& fruit: fruits) fruit->update(deltaTime);
	for (auto& power : powers) power->update(deltaTime);

	if (!activeHitbox) {
		if (timerHitbox < 0) {
			timerHitbox = TIME_HITBOX;
			activeHitbox = true;
		}
		timerHitbox -= deltaTime;
	}

	if (!activeTime) {
		if (timerTime < 0) {
			activeTime = true;
		}
		timerTime -= deltaTime;
	}

	if (!activeScore) {
		if (timerScore < 0) {
			timerScore = TIME_SCORE;
			activeScore = true;
		}
		timerScore -= deltaTime;
	}

	if (activeStop) {
		if (timerStop < 0) {
			timerStop = TIME_STOP;
			activeStop = false;
		}
		timerStop -= deltaTime;
	}

	collisionFruitPlayer();
	collisionPowerPlayer();


	if (player->isShooting()) {

		if (player->isActiveDoubleHook()) collisionBubbleDoubleHook();
		else collisionBubbleHook();

	}

	//deleteExteriorBubbles();

	if (!godMode) {
		collisionBubblePlayer();
		timerOut();
	}

	if (bubbles.size() == 0 && (lvl < 3)) {
		cout << "next lvl!" << endl;
		lvl++;
		if (hp < 3) ++hp;
		init(lvl, hp, godMode, totalPoints);
	}
	else if (bubbles.size() == 0 && lvl == 3) {
		cout << "credits!" << endl;
		finish = true;
	}
}

bool Scene::gameFinished() {
	return finish;
}

int Scene::getScore() {
	return totalPoints;
}

bool Scene::gameOver() {
	if (hp == 0) return true;
	else return false;
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	levels->render();
	map->render();
	player->render();
	for (auto& bubble : bubbles) bubble->render();
	for (auto& power : powers) power->render();
	for (auto& fruit : fruits) fruit->render();
	ui->render();


	if (mostrarPoints) {
		mostrarPoints = false;

		if (contLastBubble == 0) {
			if (lastBubble == 1) text.render(std::to_string(50), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
			if (lastBubble == 2) text.render(std::to_string(100), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
			if (lastBubble == 3) text.render(std::to_string(150), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
			if (lastBubble == 4) text.render(std::to_string(200), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
		}
		else {
			if (lastBubble == 1) text.render(std::to_string(50 * contLastBubble), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
			if (lastBubble == 2) text.render(std::to_string(100 * contLastBubble), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
			if (lastBubble == 3) text.render(std::to_string(150 * contLastBubble), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
			if (lastBubble == 4) text.render(std::to_string(200 * contLastBubble), glm::vec2(posXpoints, posYpoints), 40, glm::vec4(1, 1, 1, 1));
		}
	}

	if (mostrarFruita) {
		mostrarFruita = false;

		if (typeFruit == 0) text.render(std::to_string(500), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 1) text.render(std::to_string(1000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 2) text.render(std::to_string(2000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 3) text.render(std::to_string(3000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 4) text.render(std::to_string(4000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 5) text.render(std::to_string(5000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 6) text.render(std::to_string(6000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 7) text.render(std::to_string(7000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 8) text.render(std::to_string(8000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 9) text.render(std::to_string(9000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 10) text.render(std::to_string(10000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 11) text.render(std::to_string(12000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 12) text.render(std::to_string(14000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 13) text.render(std::to_string(16000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 14) text.render(std::to_string(18000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 15) text.render(std::to_string(20000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 16) text.render(std::to_string(22000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 17) text.render(std::to_string(24000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 18) text.render(std::to_string(26000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));
		if (typeFruit == 19) text.render(std::to_string(28000), glm::vec2(posXfruita, posYfruita), 40, glm::vec4(1, 1, 1, 1));

	}

}

void Scene::timerOut() {
	if (activeTime) {
		int time = ui->getCountDown();
		if (time == 0) {
			
			engine->play2D("sounds/Ouch.mp3");
			
			--hp;

			cout << "time" << endl;

			init(lvl, hp, god, actualPoints);
		}
	}
}

void Scene::deleteExteriorBubbles() {
	for (int index = 0; index < bubbles.size(); ++index) {
		Bubble* bubble = bubbles[index];

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		if (posX < 0 || posX > 1152) {
			if (index >= 0 && index < bubbles.size()) {
				bubbles.erase(bubbles.begin() + index);
			}
		}
		if (posY < 0 || posY > 624) {
			if (index >= 0 && index < bubbles.size()) {
				bubbles.erase(bubbles.begin() + index);
			}
		}



	}
}

void Scene::divideBubble(Bubble* bubble, int type, int index) {

	engine->play2D("sounds/BubblePop.mp3");

	srand(static_cast<unsigned int>(time(nullptr)));

	bubble->popping();

	if (type == 1) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();


		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 2, 1);
		bubble1->setPosition(glm::vec2(posX, posY));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 2, 0);
		bubble2->setPosition(glm::vec2(posX, posY));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		mostrarPoints = true;
		posXpoints = posX;
		posYpoints = posY;

		lastBubble = 1;

		if (lastBubble == 1 && contLastBubble != 0) {
			totalPoints += 50 * contLastBubble;

			if (contLastBubble == 2) contLastBubble = 4;
			else if (contLastBubble == 4) contLastBubble = 8;
		}
		else {
			
			totalPoints += 50;
			lastBubble = 1;
			contLastBubble = 2;
		}

		if ((rand() % 100) < 10) {

			Power* power = new Power();
			power->setTileMap(map);
			power->init(glm::ivec2(0, 0), texProgram, rand() % 3);
			power->setPosition(glm::vec2(posXpoints, posYpoints));

			powers.push_back(power);
		}

		if ((rand() % 100) < 100) {


			Fruit* fruit = new Fruit();
			fruit->setTileMap(map);
			fruit->init(glm::ivec2(0, 0), texProgram, rand()%20);
			fruit->setPosition(glm::vec2(posXpoints, posYpoints));

			fruits.push_back(fruit);

		}

	}
	if (type == 2) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		bubble->popping();
		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 3, 1);
		bubble1->setPosition(glm::vec2(posX, posY));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 3, 0);
		bubble2->setPosition(glm::vec2(posX, posY));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		mostrarPoints = true;
		posXpoints = posX;
		posYpoints = posY;

		lastBubble = 2;

		if (lastBubble == 2 && contLastBubble != 0) {

			totalPoints += 100 * contLastBubble;

			if (contLastBubble == 2) contLastBubble = 4;
			else if (contLastBubble == 4) contLastBubble = 8;
		}
		else {
			totalPoints += 100;
			lastBubble = 2;
			contLastBubble = 2;
		}

		if ((rand() % 100) < 20) {

			Power* power = new Power();
			power->setTileMap(map);
			power->init(glm::ivec2(0, 0), texProgram, rand() % 3);
			power->setPosition(glm::vec2(posXpoints, posYpoints));

			powers.push_back(power);
		}

		if ((rand() % 100) < 20) {


			Fruit* fruit = new Fruit();
			fruit->setTileMap(map);
			fruit->init(glm::ivec2(0, 0), texProgram, rand() % 20);
			fruit->setPosition(glm::vec2(posXpoints, posYpoints));

			fruits.push_back(fruit);

		}
	}
	if (type == 3) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		bubble->popping();
		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		Bubble* bubble1 = new Bubble();
		bubble1->init(glm::ivec2(0, 0), texProgram, 4, 1);
		bubble1->setPosition(glm::vec2(posX, posY));
		bubble1->setTileMap(map);

		bubbles.push_back(bubble1);

		Bubble* bubble2 = new Bubble();
		bubble2->init(glm::ivec2(0, 0), texProgram, 4, 0);
		bubble2->setPosition(glm::vec2(posX, posY));
		bubble2->setTileMap(map);

		bubbles.push_back(bubble2);

		mostrarPoints = true;
		posXpoints = posX;
		posYpoints = posY;

		lastBubble = 3;

		if (lastBubble == 3 && contLastBubble != 0) {

			totalPoints += 150 * contLastBubble;

			if (contLastBubble == 2) contLastBubble = 4;
			else if (contLastBubble == 4) contLastBubble = 8;
		}
		else {
			totalPoints += 150;
			lastBubble = 3;
			contLastBubble = 2;
		}

		if ((rand() % 100) < 30) {

			Power* power = new Power();
			power->setTileMap(map);
			power->init(glm::ivec2(0, 0), texProgram, rand() % 3);
			power->setPosition(glm::vec2(posXpoints, posYpoints));

			powers.push_back(power);
		}

		if ((rand() % 100) < 30) {


			Fruit* fruit = new Fruit();
			fruit->setTileMap(map);
			fruit->init(glm::ivec2(0, 0), texProgram, rand() % 20);
			fruit->setPosition(glm::vec2(posXpoints, posYpoints));

			fruits.push_back(fruit);

		}
	}
	if (type == 4) {

		int posX = bubble->getPosX();
		int posY = bubble->getPosY();

		bubble->popping();
		if (index >= 0 && index < bubbles.size()) {
			bubbles.erase(bubbles.begin() + index);
		}

		mostrarPoints = true;
		posXpoints = posX;
		posYpoints = posY;

		lastBubble = 4;

		if (lastBubble == 4 && contLastBubble != 0) {

			totalPoints += 200 * contLastBubble;

			if (contLastBubble == 2) contLastBubble = 4;
			else if (contLastBubble == 4) contLastBubble = 8;
		}
		else {
			totalPoints += 200;
			lastBubble = 4;
			contLastBubble = 2;
		}
		if ((rand() % 100) < 40) {

			Power* power = new Power();
			power->setTileMap(map);
			power->init(glm::ivec2(0, 0), texProgram, rand() % 3);
			power->setPosition(glm::vec2(posXpoints, posYpoints));

			powers.push_back(power);
		}

		if ((rand() % 100) < 40) {


			Fruit* fruit = new Fruit();
			fruit->setTileMap(map);
			fruit->init(glm::ivec2(0, 0), texProgram, rand() % 20);
			fruit->setPosition(glm::vec2(posXpoints, posYpoints));

			fruits.push_back(fruit);

		}
	}


}

void Scene::equipWeapon() {
	ui->usePower();
	if (typePower == 0) stop();
	if (typePower == 1) doubleHook(); //CAMBIAR
	if (typePower == 2) burst();
	typePower = -1;
}

void Scene::doubleHook() {
	player->setActiveDoubleHook(true);
	cout << "doubleHook" << endl;
	cout << "--------------" << endl;
}

void Scene::stop() {
	activeStop = true;
	cout << "stop" << endl;
	cout << "--------------" << endl;
}

void Scene::burst() {

	for (int index = 0; index < bubbles.size(); ++index) {
		Bubble* bubble = bubbles[index];
		int type = bubble->getType();
		
		divideBubble(bubble,type,index);

	}

	cout << "burst" << endl;
	cout << "--------------" << endl;
}

void Scene::updateTileMap(TileMap* mapV) {
	map = mapV;
	map->updateTileMap(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
}

void Scene::collisionPowerPlayer() {
	posPlayerX = player->getPosX();
	posPlayerY = player->getPosY();
	sizePlayer = player->getSize();

	for (int index = 0; index < powers.size(); ++index) {
		Power* power = powers[index];

		posPowerX = power->getPosX();
		posPowerY = power->getPosY();
		sizePower = power->getSize();
		typePower = power->getType();

		if (map->collisionFruitPlayer(posPlayerX, posPlayerY, sizePlayer, posPowerX, posPowerY, sizePower)) {


			if (typePower == 0) ui->setPower(1);
			if (typePower == 1) ui->setPower(2);
			if (typePower == 2) ui->setPower(3);


			if (index >= 0 && index < powers.size()) {
				powers.erase(powers.begin() + index);
			}

			engine->play2D("sounds/Power.mp3");

			posXpoder = posPowerX;
			posYpoder = posPowerY;

			cout << "poder obtingut!" << endl;

		}

	}
}

void Scene::collisionBubbleDoubleHook() {
	glm::ivec2 hookLeftPos = player->getLeftHookPos();
	glm::ivec2 hookRightPos = player->getRightHookPos();

	int keyFrameLeft = player->getLeftHookKeyFrame();
	int keyFrameRight = player->getRightHookKeyFrame();

	for (int index = 0; index < bubbles.size(); ++index) {
		Bubble* bubble = bubbles[index];
		int type = bubble->getType();

		posBubbleX = bubble->getPosX();
		posBubbleY = bubble->getPosY();
		sizeBubble = bubble->getSize();

		if (player->getIsShootingLeft()) {
			if (map->collisionBubbleHook(posBubbleX, posBubbleY, sizeBubble, hookLeftPos, glm::ivec2(27, 567), keyFrameLeft)) {
				divideBubble(bubble, type, index);
				player->setShootingLeftHook(false);
				break;
			}

		}

		if (player->getIsShootingRight()) {
			if (map->collisionBubbleHook(posBubbleX, posBubbleY, sizeBubble, hookRightPos, glm::ivec2(27, 567), keyFrameRight)) {
				divideBubble(bubble, type, index);
				player->setShootingRightHook(false);
				break;
			}
		}


	}



}

void Scene::collisionFruitPlayer() {
	posPlayerX = player->getPosX();
	posPlayerY = player->getPosY();
	sizePlayer = player->getSize();

	for (int index = 0; index < fruits.size(); ++index) {
		Fruit* fruit = fruits[index];

		posFruitX = fruit->getPosX();
		posFruitY = fruit->getPosY();
		sizeFruit = fruit->getSize();
		typeFruit = fruit->getType();

		if (map->collisionFruitPlayer(posPlayerX, posPlayerY, sizePlayer, posFruitX, posFruitY, sizeFruit)) {
			if (typeFruit == 0) totalPoints += 500;
			if (typeFruit == 1) totalPoints += 1000;
			if (typeFruit == 2) totalPoints += 2000;
			if (typeFruit == 3) totalPoints += 3000;
			if (typeFruit == 4) totalPoints += 4000;
			if (typeFruit == 5) totalPoints += 5000;
			if (typeFruit == 6) totalPoints += 6000;
			if (typeFruit == 7) totalPoints += 7000;
			if (typeFruit == 8) totalPoints += 8000;
			if (typeFruit == 9) totalPoints += 9000;
			if (typeFruit == 10) totalPoints += 10000;
			if (typeFruit == 11) totalPoints += 12000;
			if (typeFruit == 12) totalPoints += 14000;
			if (typeFruit == 13) totalPoints += 16000;
			if (typeFruit == 14) totalPoints += 18000;
			if (typeFruit == 15) totalPoints += 20000;
			if (typeFruit == 16) totalPoints += 22000;
			if (typeFruit == 17) totalPoints += 24000;
			if (typeFruit == 18) totalPoints += 26000;
			if (typeFruit == 19) totalPoints += 28000;

			if (index >= 0 && index < fruits.size()) {
				fruits.erase(fruits.begin() + index);
			}

			engine->play2D("sounds/Eating.mp3");

			mostrarFruita = true;
			posXfruita = posFruitX;
			posYfruita = posFruitY;

			cout << "fruita obtenida!" << endl;

		}

	}
}

void Scene::collisionBubblePlayer() {
	posPlayerX = player->getPosX();
	posPlayerY = player->getPosY();
	sizePlayer = player->getSize();

	for (auto& bubble : bubbles) {
		posBubbleX = bubble->getPosX();
		posBubbleY = bubble->getPosY();
		sizeBubble = bubble->getSize();

		if (activeHitbox && map->collisionBubblePlayer(posPlayerX, posPlayerY, sizePlayer, posBubbleX, posBubbleY, sizeBubble)) {

			engine->play2D("sounds/Ouch.mp3");
			--hp;

			init(lvl, hp, god, actualPoints);

			activeHitbox = false;
		}
	}
}

void Scene::collisionBubbleHook() {

	glm::ivec2 hookPos = player->getHookPos();
	int keyFrame = player->getHookKeyFrame();

	for (int index = 0; index < bubbles.size(); ++index) {
		Bubble* bubble = bubbles[index];
		int type = bubble->getType();

		posBubbleX = bubble->getPosX();
		posBubbleY = bubble->getPosY();
		sizeBubble = bubble->getSize();

		if (map->collisionBubbleHook(posBubbleX, posBubbleY, sizeBubble, hookPos, glm::ivec2(27, 567), keyFrame)) {
			divideBubble(bubble, type, index);
			player->setIsShooting(false);
			break;
		}
	}
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



