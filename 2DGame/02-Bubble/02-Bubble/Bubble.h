#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Bubble
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type, int dir); //type = 1 -> gran, type = 4 -> petita -- dir = 1 -> left, dir 0 -> right
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void popping();

	int getPosX();
	int getPosY();
	int getSize();
	int getType();
	int getContador();
	void setContador(int cont);



private:
	bool falling, movement, pop;
	int mida, contador;
	int direction; //1 left, 0 right
	glm::ivec2 tileMapDispl, posBubble;
	int jumpAngle, startY, alturaMax, jump, gravetatMax, contJump, energy;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	glm::vec2 tamany;


};


#endif // _PLAYER_INCLUDE


