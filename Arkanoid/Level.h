#pragma once
#include "Globals.h"

class Level {

public:
	// Constructor to initialize the level with zeros
	Level();

	// Function to draw the level on the SDL renderer
	void draw(struct SDL_Renderer* renderer);
	
private:

	constexpr int calculateBlockWidth();
	constexpr int calculateBlockHeight();

	// Level data represented as a 2D char array
	char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
};


