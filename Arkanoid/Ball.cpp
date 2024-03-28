#include "Ball.h"
#include "Globals.h"
#include <cmath>
#include "Physics.h"
#include <box2d/b2_body.h>
#include <SDL_rect.h>
#include <SDL_render.h>

void Ball::init(Physics& physics)
{
	ballPhysics = physics.addBall(PIXEL_TO_PHYSICS(400), PIXEL_TO_PHYSICS(500), PIXEL_TO_PHYSICS(5));
}

void Ball::update(SDL_Renderer* renderer, Physics& physics)
{
	auto ballPos = ballPhysics->GetPosition();
	auto ballVelocity = ballPhysics->GetLinearVelocity();
	ballVelocity.Normalize();
	ballVelocity *= 150.f;
	ballPhysics->SetLinearVelocity(ballVelocity);

	SDL_Rect boxRect;
	boxRect.x = PHYSICS_TO_PIXEL(ballPos.x); // x-coordinate of top-left corner
	boxRect.y = PHYSICS_TO_PIXEL(ballPos.y);  // y-coordinate of top-left corner
	boxRect.w = 20;  // width of the box
	boxRect.h = 20;  // height of the box

	// Set the render color (example: blue)
	SDL_SetRenderDrawColor(renderer,  0, 0, 255, 255 );

	// Draw the filled box
	SDL_RenderFillRect(renderer, &boxRect);
}
