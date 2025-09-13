#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

#define FALL_STEP 4
#define TIME_EXIT_ANIM 150
#define SIZE_PLAYER_X 66
#define SIZE_PLAYER_Y 96

enum PlayerAnims {
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, SHOOTING_LEFT, SHOOTING_RIGHT, MOVE_UP, MOVE_DOWN, EXIT_STAIRS
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	falling = true;
	inStairs = false;
	shooting = false;
	activeExitStairs = false;
	activeShootAnim = false;
	timerExitStairs = TIME_EXIT_ANIM;
	timerShootAnim = TIME_EXIT_ANIM;
	texProgram = shaderProgram;

	activeDoubleHook = false;
	shootingLeft = false;
	shootingRight = false;

	engine = SoundProgram::instance().getSoundEngine();

	spritesheet.loadFromFile("images/playerSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y), glm::vec2(0.20, 0.20), &spritesheet, &shaderProgram);

	cout << "spritePlayer: " << sprite << endl;

	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND_LEFT, 16);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 16);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 16);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.8f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 16);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.2f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.4f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.6f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0., 0.8f));

	sprite->setAnimationSpeed(SHOOTING_LEFT, 16);
	sprite->addKeyframe(SHOOTING_LEFT, glm::vec2(0.6f, 0.2f));


	sprite->setAnimationSpeed(SHOOTING_RIGHT, 16);
	sprite->addKeyframe(SHOOTING_RIGHT, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(MOVE_UP, 16);
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(MOVE_UP, glm::vec2(0.4f, 0.4f));

	sprite->setAnimationSpeed(MOVE_DOWN, 16);
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(MOVE_DOWN, glm::vec2(0.4f, 0.0f));

	sprite->setAnimationSpeed(EXIT_STAIRS, 16);
	sprite->addKeyframe(EXIT_STAIRS, glm::vec2(0.4f, 0.6f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posPlayer.x), float(tileMapDispl.y + 3 * posPlayer.y)));

	hook = new Hook();
	hook->init(glm::vec2(0), texProgram);
	hook->setTileMap(map);

	doubleHookLeft = new Hook();
	doubleHookLeft->init(glm::vec2(0), texProgram);
	doubleHookLeft->setTileMap(map);
	doubleHookRight = new Hook();
	doubleHookRight->init(glm::vec2(0), texProgram);
	doubleHookRight->setTileMap(map);
}


void Player::update(int deltaTime)
{
	if (shooting) {

		if (activeDoubleHook) {

			bool stopLeft = doubleHookLeft->stopShooting();
			bool stopRight = doubleHookRight->stopShooting();

			TileMap* newTileMap;

			if (stopLeft) {
				shootingLeft = false;
				newTileMap = doubleHookLeft->updateIfMapDestroyed();

				if (newTileMap != NULL) {
					setTileMap(newTileMap);
					Game::instance().updateTileMap(newTileMap);
				}

			}
			else {
				doubleHookLeft->update(deltaTime);
				shootingLeft = true;
			}

			if (stopRight) {
				shootingRight = false;
				newTileMap = doubleHookRight->updateIfMapDestroyed();

				if (newTileMap != NULL) {
					setTileMap(newTileMap);
					Game::instance().updateTileMap(newTileMap);
				}

			}
			else {
				doubleHookRight->update(deltaTime);
				shootingRight = true;
			}

			if (stopLeft && stopRight) activeDoubleHook = false;

		}
		else {

			if (hook->stopShooting()) {
				shooting = false;

				TileMap* newTileMap = hook->updateIfMapDestroyed();

				if (newTileMap != NULL) {
					setTileMap(newTileMap);
					Game::instance().updateTileMap(newTileMap);
				}

			}
			else if (!shootingRight)  hook->update(deltaTime);
		}
	}

	sprite->update(deltaTime);

	if (activeExitStairs) {
		if (timerExitStairs < 0) {
			timerExitStairs = TIME_EXIT_ANIM;
			playerOrientation();
			activeExitStairs = false;

		}
		else timerExitStairs -= deltaTime;
	}

	if (activeShootAnim) {
		if (timerShootAnim < 0) {
			timerShootAnim = TIME_EXIT_ANIM;
			playerOrientation();
			activeShootAnim = false;

		}
		else  timerShootAnim -= deltaTime;
	}

	// DISPARAR
	if (Game::instance().getKey(GLFW_KEY_SPACE) && !inStairs && !activeExitStairs && !shooting) {
		if (left_orientation && sprite->animation() != SHOOTING_LEFT)
			sprite->changeAnimation(SHOOTING_LEFT);
		else if (right_orientation && sprite->animation() != SHOOTING_RIGHT)
			sprite->changeAnimation(SHOOTING_RIGHT);
		shooting = true;
		activeShootAnim = true;

		if (activeDoubleHook) {
			if (!shootingLeft) doubleHookLeft->setPosition(glm::vec2(posPlayer.x - SIZE_PLAYER_X, posPlayer.y));
			if (!shootingRight) doubleHookRight->setPosition(glm::vec2(posPlayer.x + SIZE_PLAYER_X, posPlayer.y));

		}
		else hook->setPosition(glm::vec2(posPlayer));



		engine->play2D("sounds/Shoot.mp3");

		// MOURE ESQUERRA
	}
	else if (Game::instance().getKey(GLFW_KEY_LEFT) && !inStairs && !activeExitStairs && !activeShootAnim) {
		posPlayer.x -= 6;

		left_orientation = true;
		right_orientation = false;

		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y))) {
			posPlayer.x += 6;
			sprite->changeAnimation(STAND_LEFT);
		}

		// MOURE DRETA
	}
	else if (Game::instance().getKey(GLFW_KEY_RIGHT) && !inStairs && !activeExitStairs && !activeShootAnim) {
		posPlayer.x += 6;

		left_orientation = false;
		right_orientation = true;

		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		if (map->collisionMoveRight(posPlayer, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y))) {
			posPlayer.x -= 6;
			sprite->changeAnimation(STAND_RIGHT);
		}

		// MOURE AMUNT
	}
	else if (Game::instance().getKey(GLFW_KEY_UP) && !activeExitStairs && !activeShootAnim) {

		//Aixo ho fem per evitar moure el personatge abans de saber que fara
		glm::ivec2 posAux = posPlayer;
		int newPos;
		posAux.y -= 4;

		//Sortir d'escales
		if (inStairs && map->exitUpStairs(posAux, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y))) {
			inStairs = false;
			activeExitStairs = true;
			posPlayer.y = posPlayer.y - SIZE_PLAYER_Y / 2 - 4;
			sprite->changeAnimation(EXIT_STAIRS);

			//Pujar escales
		}
		else if (map->collisionStairs(posAux, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y), newPos)) {
			inStairs = true;
			posPlayer.y -= 4; //Aquest -4 s'ha afegit perque es detecta 4 pixels abans quan ha de pujar escales. Es per evitar bug d'animacio!
			posPlayer.x = newPos; //Ajustar posicio del jugador a l'escala
			if (sprite->animation() != MOVE_UP) sprite->changeAnimation(MOVE_UP);
		}

		// MOURE A BAIX
	}
	else if (Game::instance().getKey(GLFW_KEY_DOWN) && !activeExitStairs && !activeShootAnim) {

		//Aixo ho fem per evitar moure el personatge abans de saber que fara
		glm::ivec2 posAux = posPlayer;
		posAux.y += 4;

		int newPos;
		bool collisionPostMove = map->collisionStairs(posAux, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y), newPos);
		bool exitStairs = map->exitDownStairs(posAux, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y));

		//Pot baixar escales
		if (collisionPostMove && !exitStairs) {

			//Entra a les escales per amunt o segueix baixant
			if (!inStairs) posPlayer.y += (SIZE_PLAYER_Y / 2);
			else posPlayer.y += 4;

			posPlayer.x = newPos; //Ajustar posicio del jugador a l'escala
			inStairs = true;
			if (sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);

			//No pot baixar escales o surt d'elles
		}
		else if (exitStairs) {
			inStairs = false;
			activeExitStairs = true;
			playerOrientation();
		}


	}
	else if (!activeExitStairs) {
		if (!inStairs) playerOrientation();
		else sprite->update(-deltaTime);
	}

	//CODI DE CAIGUDA DEL PERSONATGE
	if (!falling && !inStairs && !map->collisionMoveDown(posPlayer, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y), &posPlayer.y)) {
		falling = true;

	}
	else if (!inStairs) {

		posPlayer.y += FALL_STEP;

		if (map->collisionMoveDown(posPlayer, glm::ivec2(SIZE_PLAYER_X, SIZE_PLAYER_Y), &posPlayer.y)) {
			falling = false;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//hook->shoot(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	if (shooting) {
		if (activeDoubleHook) {
			if (shootingLeft) doubleHookLeft->render();
			if (shootingRight) doubleHookRight->render();

		}
		else hook->render();
	}

	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::playerOrientation() {
	if (left_orientation && sprite->animation() != STAND_LEFT) {
		sprite->changeAnimation(STAND_LEFT);

	}
	else if (right_orientation && sprite->animation() != STAND_RIGHT) {
		sprite->changeAnimation(STAND_RIGHT);
	}
}

int Player::getPosX() {
	return posPlayer.x;
}

int Player::getPosY() {
	return posPlayer.y;
}

int Player::getSize() {
	return 96;
}

glm::ivec2 Player::getHookPos() {
	return glm::ivec2(hook->getPosX(), hook->getPosY());
}

int Player::getHookKeyFrame() {
	return hook->getKeyFrame();
}

int Player::getLeftHookKeyFrame() {
	return doubleHookLeft->getKeyFrame();
}
int Player::getRightHookKeyFrame() {
	return doubleHookRight->getKeyFrame();
}

bool Player::isShooting() {
	return shooting;
}

void Player::setIsShooting(bool val) {
	shooting = false;
}

void Player::setActiveDoubleHook(bool val) {
	activeDoubleHook = val;
}

glm::ivec2 Player::getLeftHookPos() {
	return glm::ivec2(doubleHookLeft->getPosX(), doubleHookLeft->getPosY());

}
glm::ivec2 Player::getRightHookPos() {
	return glm::ivec2(doubleHookRight->getPosX(), doubleHookRight->getPosY());

}

bool Player::isActiveDoubleHook() {
	return activeDoubleHook;
}

void Player::setShootingLeftHook(bool value) {
	shootingLeft = value;

}
void Player::setShootingRightHook(bool value) {
	shootingRight = value;
}

bool Player::getIsShootingLeft() { return shootingLeft; }
bool Player::getIsShootingRight() { return shootingRight; }