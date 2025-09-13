#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Hook.h"
#include "SoundProgram.h"
#include <irrKlang.h>
using namespace irrklang;


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getLeftHookPos();
	glm::ivec2 getRightHookPos();

	void setShootingLeftHook(bool value);
	void setShootingRightHook(bool value);

	int getLeftHookKeyFrame();
	int getRightHookKeyFrame();

	bool isActiveDoubleHook();

	bool getIsShootingLeft();
	bool getIsShootingRight();

	void setActiveDoubleHook(bool val);

	int getPosX();
	int getPosY();
	int getSize();
	int getHookKeyFrame();

	bool isShooting();
	void setIsShooting(bool val);


	glm::ivec2 getHookPos();
	
private:
	bool falling, inStairs, activeExitStairs, activeShootAnim, left_orientation, right_orientation, shooting;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	int timerExitStairs;
	int timerShootAnim;

	bool shootingLeft, shootingRight, activeDoubleHook;

	ISoundEngine* engine;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	Hook* hook;

	Hook* doubleHookLeft;
	Hook* doubleHookRight;

	ShaderProgram texProgram;

	void playerOrientation();

};


#endif // _PLAYER_INCLUDE


