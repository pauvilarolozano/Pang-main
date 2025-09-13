#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"


#define JUMP_ANGLE_STEP 1
#define FALL_STEP 4


enum BubbleAnims
{
	JUMPING, POPPING
};


void Bubble::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type, int dir)
{
	falling = false;

	pop = false;
	movement = true;
	mida = type;
	contador = 4;

	direction = dir;


	jump = -4;
	gravetatMax = 8;

	energy = 0;
	contJump = 0;


	if (type == 1) {
		tamany = glm::ivec2(128, 128);
		alturaMax = 16;
	}
	if (type == 2) {
		tamany = glm::ivec2(92, 92);
		alturaMax = 100;
	}
	if (type == 3) {
		tamany = glm::ivec2(64, 64);
		alturaMax = 200;
	}
	if (type == 4) {
		tamany = glm::ivec2(40, 40);
		alturaMax = 300;
	}


	spritesheet.loadFromFile("images/BubbleAnimations.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(tamany, glm::vec2(0.2, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(JUMPING, 8);
	sprite->addKeyframe(JUMPING, glm::vec2(0.0f, 0.f));

	sprite->setAnimationSpeed(POPPING, 8);
	sprite->addKeyframe(POPPING, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(POPPING, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(POPPING, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(POPPING, glm::vec2(0.8f, 0.f));


	sprite->changeAnimation(JUMPING);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posBubble.x), float(tileMapDispl.y + 3 * posBubble.y)));

}

int Bubble::getPosX() {
	return posBubble.x;
}

int Bubble::getPosY() {
	return posBubble.y;
}

int Bubble::getSize() {
	return tamany.x;
}

int Bubble::getType() {
	return mida;
}

void Bubble::update(int deltaTime)
{

	if (pop) {
		sprite->update(deltaTime);
		return;
	}

	sprite->update(deltaTime);

	posBubble.y += jump;

	if (energy >= 0) {
		if (jump < 0) {
			jump = 0;
		}

		contJump += 2;

		if (contJump > 4) {
			contJump = 0;
			jump = (jump + 1 > gravetatMax) ? gravetatMax : jump + 1;
		}
	}
	else {
		int velParabola = energy / 4;
		jump = (velParabola < -gravetatMax) ? -gravetatMax : velParabola;
		jump = (velParabola > -1) ? -1 : jump;
		energy = alturaMax - posBubble.y;
	}


		if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
		{
			while (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
				posBubble.y -= 1;
			jump = 0;
			energy = alturaMax - posBubble.y;
		}

		if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
		{
			while (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
				posBubble.y += 1;
			jump = 0;
			energy = 0;
		}

		if (posBubble.y > 570) posBubble.y = 560;
		if (posBubble.y < 10) posBubble.y = 20;

		if (direction == 0) posBubble.x += 4;
		else posBubble.x -= 4;

		if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
			while (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x))
				posBubble.x -= 1;
			direction = 1;
		}
		if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
			while (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x))
				posBubble.x += 1;
			direction = 0;
		}



		if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
					posBubble.y -= 1;
				jump = 0;
				energy = alturaMax - posBubble.y;

				if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x))
						posBubble.x -= 1;
					direction = 1;
				}
			}
		}

		if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveDown(posBubble, tamany, &posBubble.y))
					posBubble.y -= 1;
				jump = 0;
				energy = alturaMax - posBubble.y;


				if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x))
						posBubble.x += 1;
					direction = 0;
				}
			}
		}

		if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
					posBubble.y += 1;
				jump = 0;
				energy = 0;


				if (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveRight(posBubble, tamany, &posBubble.x))
						posBubble.x -= 1;
					direction = 1;
				}
			}
		}

		if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y) && map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
			if (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y)) {
				while (map->bubbleCollisionMoveUp(posBubble, tamany, &posBubble.y))
					posBubble.y += 1;
				jump = 0;
				energy = 0;

				if (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x)) {
					while (map->bubbleCollisionMoveLeft(posBubble, tamany, &posBubble.x))
						posBubble.x += 1;
					direction = 0;
				}
			}
		}
		
		if (posBubble.x < 10) posBubble.x = 20;
		if (posBubble.x > 1100) posBubble.x = 1092;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}

void Bubble::render()
{
	sprite->render();
}

void Bubble::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bubble::setPosition(const glm::vec2& pos)
{
	posBubble = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBubble.x), float(tileMapDispl.y + posBubble.y)));
}

void Bubble::popping() {
	pop = true;
	sprite->changeAnimation(POPPING);
}

int Bubble::getContador() {
	return contador;
}

void Bubble::setContador(int cont) {
	contador = cont;
}

