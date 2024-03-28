#pragma once
#include "Globals.h"

class Level {

public:

	bool init(struct SDL_Renderer* renderer, class Physics& physics);
	
	void draw(struct SDL_Renderer* renderer, class Physics& physics);
	
private:

	// Level data represented as a 2D char array
	char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
	class b2Body* levelBodies[LEVEL_HEIGHT][LEVEL_WIDTH];

	int testX = 400, testY = 500;
	
	struct SDL_Texture* texture = nullptr;
};


