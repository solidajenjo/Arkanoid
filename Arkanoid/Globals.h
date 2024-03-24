#pragma once

#define TARGET_FRAME_TIME 16
#define UI_COLOR { 68, 242, 5 }
#define BRICK_TEX_RECT { 175, 83, 15, 7 }
#define GAME_TEXTURE "SNES_ Arkanoid.BMP"

//Level dimensions
#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 30

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


//Physics
#define PHYSICS_TIMESTEP .016f
#define PHYSICS_VELOCITY_ITERATIONS 6
#define PHYSICS_POSITION_ITERATIONS 2
#define PHYSICS_BRICK_LINEAR_DAMPING 0

#define RADTODEG(x) ((x) * (180.0f / M_PI))
#define DEGTORAD(x) ((x) * (M_PI / 180.0f))