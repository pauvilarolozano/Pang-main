#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Fruit.h"
#include "Game.h"

#define FALL_STEP 4

#define SIZE_FRUIT_X 48
#define SIZE_FRUIT_Y 48

enum FruitAnim {
	FALLING
};

void Fruit::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int fruitType) {
	falling = true;

	texProgram = shaderProgram;

	type = fruitType;

	spritesheet.loadFromFile("images/Fruit.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(SIZE_FRUIT_X, SIZE_FRUIT_Y), glm::vec2(0.1, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	if (fruitType == 0) {
		sprite->setAnimationSpeed(FALLING, 8);
		sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.f));

	}
	if (fruitType == 1) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.1f, 0.f));
	}
	if (fruitType == 2) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.2f, 0.f));
	}
	if (fruitType == 3) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.3f, 0.f));
	}
	if (fruitType == 4) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.4f, 0.f));
	}
	if (fruitType == 5) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.5f, 0.f));
	}
	if (fruitType == 6) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.6f, 0.f));
	}
	if (fruitType == 7) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.7f, 0.f));
	}
	if (fruitType == 8) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.8f, 0.f));
	}
	if (fruitType == 9) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.9f, 0.f));
	}
	if (fruitType == 10) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.5f));
	}
	if (fruitType == 11) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.1f, 0.5f));
	}
	if (fruitType == 12) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.2f, 0.5f));
	}
	if (fruitType == 13) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.3f, 0.5f));
	}
	if (fruitType == 14) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.4f, 0.5f));
	}
	if (fruitType == 15) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.5f, 0.5f));
	}
	if (fruitType == 16) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.6f, 0.5f));
	}
	if (fruitType == 17) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.7f, 0.5f));
	}
	if (fruitType == 18) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.8f, 0.5f));
	}
	if (fruitType == 19) {
		sprite->setAnimationSpeed(FALLING, 16);
		sprite->addKeyframe(FALLING, glm::vec2(0.9f, 0.5f));
	}

	sprite->changeAnimation(FALLING);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posFruit.x), float(tileMapDispl.y + 3 * posFruit.y)));


}

void Fruit::update(int deltaTime)
{

	sprite->update(deltaTime);

	//cout << posFruit.x << " " << posFruit.y << endl;

	if (falling) {
		posFruit.y += 4;
		sprite->changeAnimation(FALLING);
	}

	if (map->collisionMoveDown(posFruit, glm::ivec2(SIZE_FRUIT_X, SIZE_FRUIT_Y), &posFruit.y)) {
		falling = false;
	}
	else falling = true;


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFruit.x), float(tileMapDispl.y + posFruit.y)));

}

void Fruit::render()
{
	sprite->render();
}

void Fruit::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Fruit::setPosition(const glm::vec2& pos)
{
	posFruit = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFruit.x), float(tileMapDispl.y + posFruit.y)));
}

int Fruit::getPosX() {
	return posFruit.x;
}

int Fruit::getPosY() {
	return posFruit.y;
}

int Fruit::getSize() {
	return 32;
}

int Fruit::getType() {
	return type;
}
