#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	void updateTileMap(const glm::vec2& minCoords, ShaderProgram& program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool bubbleCollisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool bubbleCollisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool bubbleCollisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool bubbleCollisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool bubbleCollisionFloor(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionStairs(const glm::ivec2& pos, const glm::ivec2& size, int &posX) const;
	bool exitDownStairs(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool exitUpStairs(const glm::ivec2& pos, const glm::ivec2& size) const;

	bool collisionFruitPlayer(int& posPlayerX, int& posPlayerY, int& sizePlayer, int& posFruitX, int& posFruitY, int& sizeFruit) const;
	bool collisionBubblePlayer(int& posPlayerX, int& posPlayerY, int& sizePlayer, int& posBubbleX, int& posBubbleY, int& sizeBubble) const;
	bool collisionBubbleHook(int& posBubbleX, int& posBubbleY, int& sizeBubbl, const glm::ivec2& pos, const glm::ivec2& size, const int keyFrame) const;
	bool collisionBubbleHook2(int x0, int x1, int y0, int y1, int xBubble, int yBubble) const;


	bool collisionHook(const glm::ivec2& pos, const glm::ivec2& size, const int keyFrame) const;
	TileMap* collisionHookNewMap(const glm::ivec2& pos, const glm::ivec2& size, const int keyFrame);

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

};


#endif // _TILE_MAP_INCLUDE


