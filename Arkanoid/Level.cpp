#include "Level.h"
#include "SDL.h"
#include "box2d/box2d.h" //TODO:REMOVE PHYSICS IN THIS FILE
#include "Physics.h"
#include <cmath>

bool Level::init(SDL_Renderer* renderer, Physics& physics)
{
    const char* levelRows[] = {
        "00000000000000000000",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "00000000000000000000",
        "00000000000000000000",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "00000000000000000000",
        "00000000000000000000",
        "00111111111111111100",
        "00111111111111111100",
        "01111111111111111110",
        "01111111111111111110",
        "01111000000000011110",
        "01111000000000011110",
        "00100001111110000100",
        "00100000011000000100",
        "00011000000000011000",
        "00000000000000000000",
        "00000000000000000000",
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
                levelBodies[y][x] = physics.addBrick(
                    PIXEL_TO_PHYSICS(x * BLOCK_WIDTH + BLOCK_WIDTH / 2),
                    PIXEL_TO_PHYSICS(y * BLOCK_HEIGHT + BLOCK_HEIGHT / 2),
                    PIXEL_TO_PHYSICS(BLOCK_WIDTH),
                    PIXEL_TO_PHYSICS(BLOCK_HEIGHT),
                    false);
            }
            else {
                levelBodies[y][x] = nullptr;
            }

        }
    }
    
    //Side and top margins they don't need to be tracked, Sides are slightly rotated to prevent endless horizontal bouncing, also the top
    //LEFT
    physics.addBrick(1, PIXEL_TO_PHYSICS(SCREEN_HEIGHT / 2), PIXEL_TO_PHYSICS(5), PIXEL_TO_PHYSICS(SCREEN_HEIGHT), true, 0.03f);
    //RIGHT
    physics.addBrick(PIXEL_TO_PHYSICS(SCREEN_WIDTH) - 1, PIXEL_TO_PHYSICS(SCREEN_HEIGHT / 2), PIXEL_TO_PHYSICS(5), PIXEL_TO_PHYSICS(SCREEN_HEIGHT), true, -0.03f);
    //TOP
    physics.addBrick(PIXEL_TO_PHYSICS(SCREEN_WIDTH / 2), 0, PIXEL_TO_PHYSICS(SCREEN_WIDTH), PIXEL_TO_PHYSICS(5), true, 0.01f);
    //BOTTOM
    physics.addBrick(PIXEL_TO_PHYSICS(SCREEN_WIDTH / 2), PIXEL_TO_PHYSICS(SCREEN_HEIGHT), PIXEL_TO_PHYSICS(SCREEN_WIDTH), PIXEL_TO_PHYSICS(5), true);
 
    
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
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            if (auto brickPhysics = levelBodies[y][x]) {
                /*
                auto brickContacts = brickPhysics->GetContactList();
                if (rand() % 10000 < 10 || (brickContacts && brickContacts->contact->GetFixtureA()->GetBody()->GetLinearVelocity().Length() > 1)) {
                    brickPhysics->ApplyLinearImpulse({ static_cast<float>((rand() % 2000) - 1000), 0 }, brickPhysics->GetPosition(), true);                    
                }
                */
                auto brickPos = brickPhysics->GetPosition();
                auto brickRot = brickPhysics->GetAngle();

                SDL_Rect srcRect = BRICK_TEX_RECT;
                SDL_Rect blockRect = { static_cast<int>(PHYSICS_TO_PIXEL(brickPos.x)) - BLOCK_WIDTH / 2, static_cast<int>(PHYSICS_TO_PIXEL(brickPos.y)) - BLOCK_HEIGHT / 2, BLOCK_WIDTH, BLOCK_HEIGHT};
                
                SDL_RenderCopyEx(renderer, texture, &srcRect, &blockRect, RADTODEG(brickRot), NULL, SDL_RendererFlip());
            }
        }
    }
}
