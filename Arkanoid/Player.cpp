#include "Player.h"
#include "Globals.h"
#include "Physics.h"
#include <SDL_surface.h>
#include <SDL_render.h>
#include "box2d/b2_body.h"

bool Player::init(SDL_Renderer* renderer, Physics& physics)
{
    playerPhysics = physics.addPlayer(PIXEL_TO_PHYSICS(400), PIXEL_TO_PHYSICS(550), PIXEL_TO_PHYSICS(40), PIXEL_TO_PHYSICS(10));
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

void Player::draw(SDL_Renderer* renderer)
{
    auto brickPos = playerPhysics->GetPosition();
    auto brickRot = playerPhysics->GetAngle();

    SDL_Rect srcRect = PLAYER_TEX_RECT;
    SDL_Rect playerRect = { static_cast<int>(PHYSICS_TO_PIXEL(brickPos.x)) - PLAYER_WIDTH / 2, static_cast<int>(PHYSICS_TO_PIXEL(brickPos.y)) - PLAYER_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT };

    SDL_RenderCopyEx(renderer, texture, &srcRect, &playerRect, 0, NULL, SDL_RendererFlip());
}

void Player::update(int xDir)
{
    playerPhysics->ApplyForce({ static_cast<float>(xDir) * 500000.f, 0.f }, playerPhysics->GetPosition(), true);
}
