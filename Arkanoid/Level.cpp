#include "Level.h"
#include "SDL.h"
#include "box2d/box2d.h" 
#include "Physics.h"
#include <cmath>

bool Level::init(SDL_Renderer* renderer, Physics& physics)
{
    const char* levelRows[] = {
        "00000000000000000000",
        "00000000000000000000",
        "00000000000000000000",
        "00000000000000000000",
        "00000000000000000000",
        "00111111111111111100",
        "00111111111111111100",
        "00000000000000000000",
        "00000000000000000000",
        "00111111111111111100",
        "00111111111111111100",
        "00100000000000000100",
        "00111111111111111100",
        "00111111111111111100",
        "00000000000000000000",
        "00000000000000000000",
        "00111111111111111100",
        "00111111111111111100",
        "00100000000000000100",
        "00111111111111111100",
        "00111000000000011100",
        "00111000000000011100",
        "00100001111110000100",
        "00100000011000000100",
        "00011000000000011000",
        "00000000000000000000",
        "00000000000000000000",
        "00000000000000000000",
        "00000000000000000000",
        "00000000000000000000"
    };
    
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            levelData[y][x] = levelRows[y][x];
            if (levelData[y][x] != '0') {
                physics.addBrick(
                    PIXEL_TO_PHYSICS(x * BLOCK_WIDTH + BLOCK_WIDTH / 2),
                    PIXEL_TO_PHYSICS(y * BLOCK_HEIGHT + BLOCK_HEIGHT / 2),
                    PIXEL_TO_PHYSICS(BLOCK_WIDTH),
                    PIXEL_TO_PHYSICS(BLOCK_HEIGHT),
                    b2_kinematicBody);
            }            
        }
    }
    
    //Side and top margins they don't need to be tracked, Sides are slightly rotated to prevent endless horizontal bouncing, also the top
    //LEFT
    physics.addBrick(1, PIXEL_TO_PHYSICS(SCREEN_HEIGHT / 2), PIXEL_TO_PHYSICS(5), PIXEL_TO_PHYSICS(SCREEN_HEIGHT), b2_staticBody, 0.03f);
    //RIGHT
    physics.addBrick(PIXEL_TO_PHYSICS(SCREEN_WIDTH) - 1, PIXEL_TO_PHYSICS(SCREEN_HEIGHT / 2), PIXEL_TO_PHYSICS(5), PIXEL_TO_PHYSICS(SCREEN_HEIGHT), b2_staticBody, -0.03f);
    //TOP
    physics.addBrick(PIXEL_TO_PHYSICS(SCREEN_WIDTH / 2), 0, PIXEL_TO_PHYSICS(SCREEN_WIDTH), PIXEL_TO_PHYSICS(5), b2_staticBody, 0.01f);
    //BOTTOM - Kill zone: it handles destruction of objects on the bottom of the level
    physics.addKillZone(PIXEL_TO_PHYSICS(SCREEN_WIDTH / 2), PIXEL_TO_PHYSICS(SCREEN_HEIGHT), PIXEL_TO_PHYSICS(SCREEN_WIDTH + 20), PIXEL_TO_PHYSICS(5), b2_staticBody, 0.005f);
 
    
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

void Level::draw(SDL_Renderer* renderer, Physics& physics)
{
    for (auto brickPhysics : physics.getBricks()) {
        auto brickPos = brickPhysics->GetPosition();
        auto brickRot = brickPhysics->GetAngle();

        SDL_Rect srcRect = BRICK_TEX_RECT;
        SDL_Rect blockRect = { static_cast<int>(PHYSICS_TO_PIXEL(brickPos.x)) - BLOCK_WIDTH / 2, static_cast<int>(PHYSICS_TO_PIXEL(brickPos.y)) - BLOCK_HEIGHT / 2, BLOCK_WIDTH, BLOCK_HEIGHT};
                
        SDL_RenderCopyEx(renderer, texture, &srcRect, &blockRect, RADTODEG(brickRot), NULL, SDL_RendererFlip());
    }
}
