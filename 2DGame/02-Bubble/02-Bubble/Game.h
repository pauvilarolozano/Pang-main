#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Menu.h"
#include "SoundProgram.h"


#define SCREEN_WIDTH 1152
#define SCREEN_HEIGHT 740


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:

	~Game();
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void updateTileMap(TileMap* mapV);
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

private:
	bool bPlay; // Continue to play game?
	bool start;
	int viewType; // menu = 0, instructions = 1, game = 2, gameOver = 3, credits = 4
	int level; // level1 = 1, level2 = 2, level3 = 3
	bool level1, level2, level3;
	int lives;
	bool gameOver, gameFinished;
	bool godMode;

	int point;

	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	Scene *scene;
	Menu *menu;
	ISoundEngine* engine;

};


#endif // _GAME_INCLUDE


