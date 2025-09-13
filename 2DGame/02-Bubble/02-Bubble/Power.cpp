#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Power.h"

#define FALL_STEP 4

#define SIZE_POWER_X 48
#define SIZE_POWER_Y 48

enum PowerAnims {
	FALLING, NOFALLING
};

void Power::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int powerType) {
	falling = true;

	type = powerType;

	spritesheet.loadFromFile("images/Power.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(SIZE_POWER_X, SIZE_POWER_Y), glm::vec2(0.33, 1), &spritesheet, &shaderProgram);

	cout << "spritePower: " << sprite << endl;

	sprite->setNumberAnimations(2);

	if (powerType == 0) {
	sprite->setAnimationSpeed(FALLING, 16);
	sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.f));
	}
	if (powerType == 1) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.33f, 0.f));
	}
	if (powerType == 2) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.66f, 0.f));
	}
	

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(posPower.x), float(posPower.y)));


}

void Power::update(int deltaTime)
{

	sprite->update(deltaTime);

	if (falling) {
		posPower.y += 4;
	}

	if (map->collisionMoveDown(posPower, glm::ivec2(SIZE_POWER_X, SIZE_POWER_Y), &posPower.y)) {
		falling = false;
	}
	else falling = true;


	sprite->setPosition(glm::vec2(float(posPower.x), float(posPower.y)));

}

void Power::render()
{
	sprite->render();
}

void Power::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Power::setPosition(const glm::vec2& pos)
{
	posPower = pos;
	sprite->setPosition(glm::vec2(float(posPower.x), float(posPower.y)));
}

int Power::getPosX() {
	return posPower.x;
}

int Power::getPosY() {
	return posPower.y;
}

int Power::getSize() {
	return 38;
}

int Power::getType() {
	return type;
}
