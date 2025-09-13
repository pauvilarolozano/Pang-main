#ifndef _FRUIT_INCLUDE
#define _FRUIT_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Fruit
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int fruitType);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	int getPosX();
	int getPosY();
	int getSize();
	int getType();


private:
	bool falling;
	int type;
	glm::ivec2 tileMapDispl, posFruit;

	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	ShaderProgram texProgram;


};


#endif // _FRUIT_INCLUDE


