#ifndef _HOOK_INCLUDE
#define _HOOK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "SoundProgram.h"
#include <irrKlang.h>
using namespace irrklang;

class Hook
{

public:

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	bool stopShooting();
	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	int getPosX();
	int getPosY();
	int getKeyFrame();


	TileMap* updateIfMapDestroyed();

private:

	ISoundEngine* engine;
	glm::ivec2 tileMapDispl, posHook;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};




#endif // _HOOK_INCLUDE