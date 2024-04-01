#pragma once

#define TARGET_FRAME_TIME 16
#define UI_COLOR { 68, 242, 5 }
#define BRICK_TEX_RECT { 175, 83, 15, 7 }
#define GAME_TEXTURE "SNES_ Arkanoid.BMP"

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//Level dimensions
#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 30
#define BLOCK_WIDTH SCREEN_WIDTH / LEVEL_WIDTH
#define BLOCK_HEIGHT (SCREEN_HEIGHT / LEVEL_HEIGHT) * .6f

//Physics
#define GRAVITY 0.f, 10.f
#define PHYSICS_TIMESTEP .016f
#define PHYSICS_VELOCITY_ITERATIONS 8
#define PHYSICS_POSITION_ITERATIONS 2
#define PHYSICS_BRICK_LINEAR_DAMPING .0001f
#define PHYSICS_BRICK_ANGULAR_DAMPING .1f
#define BRICK_DENSITY 5.f
#define BALL_DENSITY 10.f
#define BALL_RESTITUTION 1.f
#define BALL_SPEED 50.f

#define RADTODEG(x) ((x) * (180.0f / M_PI))
#define DEGTORAD(x) ((x) * (M_PI / 180.0f))

#define PHYSICS_SCALE 10.f
#define PHYSICS_TO_PIXEL(x) x * PHYSICS_SCALE
#define PIXEL_TO_PHYSICS(x) static_cast<float>(x) / PHYSICS_SCALE