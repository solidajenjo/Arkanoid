#include "Ball.h"
#include "Globals.h"
#include <cmath>
#include "Physics.h"
#include <box2d/b2_body.h>
#include <SDL_rect.h>
#include <SDL_render.h>

bool Ball::init(struct SDL_Renderer* renderer, Physics& physics)
{
	ballPhysics = physics.addBall(PIXEL_TO_PHYSICS(400), PIXEL_TO_PHYSICS(500), PIXEL_TO_PHYSICS(5));
	
	SDL_Surface* surface = SDL_LoadBMP(GAME_TEXTURE);
	if (!surface) {
		return false;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		return false;
	}

	return true;
}

void Ball::update(SDL_Renderer* renderer, Physics& physics)
{
	auto ballPos = ballPhysics->GetPosition();
	auto ballVelocity = ballPhysics->GetLinearVelocity();
	ballVelocity.Normalize();
	ballVelocity *= BALL_SPEED;
	ballPhysics->SetLinearVelocity(ballVelocity);

	SDL_Rect srcRect = BALL_TEX_RECT;
	SDL_Rect ballRect = { static_cast<int>(PHYSICS_TO_PIXEL(ballPos.x)) - BALL_WIDTH / 2, static_cast<int>(PHYSICS_TO_PIXEL(ballPos.y)) - BALL_HEIGHT / 2, BALL_WIDTH, BALL_HEIGHT };

	SDL_RenderCopyEx(renderer, texture, &srcRect, &ballRect, 0, NULL, SDL_RendererFlip());
}
