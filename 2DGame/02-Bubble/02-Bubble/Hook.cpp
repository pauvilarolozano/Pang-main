#include <iostream>
#include <GL/glew.h>
#include "Hook.h"

enum HookAnims {
	ACTION
};

void Hook::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {

	spritesheet.loadFromFile("images/hookSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(27, 567), glm::vec2(1.0 / 70.0, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ACTION, 64);

	engine = SoundProgram::instance().getSoundEngine();

	for (int i = 0; i <= 69; i++)
		sprite->addKeyframe(ACTION, glm::vec2(i / 70.0f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 3 * posHook.x), float(tileMapDispl.y + 3 * posHook.y)));
}


void Hook::update(int deltaTime) {
	sprite->update(deltaTime);
}

bool Hook::stopShooting() {
	int keyFrame = sprite->getCurrentKeyFrame();  //DEL 0 al 69
	return keyFrame == 69 || map->collisionHook(posHook, glm::ivec2(27, 567), keyFrame);
}


TileMap* Hook::updateIfMapDestroyed() {
	int keyFrame = sprite->getCurrentKeyFrame();  //DEL 0 al 69
	TileMap* newMap = map->collisionHookNewMap(posHook, glm::ivec2(27, 567), keyFrame);

	if (newMap != NULL) {
		engine->play2D("sounds/Glass.mp3");
		map = newMap;
	}

	return newMap;
}

void Hook::render() {
	sprite->render();
}

void Hook::setTileMap(TileMap* tileMap) {
	map = tileMap;
}

void Hook::setPosition(const glm::vec2& pos) {
	posHook = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posHook.x + 32), float(tileMapDispl.y + posHook.y - (567 - 96))));
	sprite->changeAnimation(ACTION);
}

int Hook::getPosX() {
	return posHook.x;
}

int Hook::getPosY() {
	return posHook.y;
}


int Hook::getKeyFrame() {
	return sprite->getCurrentKeyFrame();
}
