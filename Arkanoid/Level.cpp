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

    auto blockHeight = calculateBlockHeight();
    auto blockWidth = calculateBlockWidth();

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            levelData[y][x] = levelRows[y][x];
            if (levelData[y][x] != '0') {
                levelBodies[y][x] = physics.addBrick(x * blockWidth + blockWidth / 2, y * blockHeight + blockHeight / 2, calculateBlockWidth(), calculateBlockHeight());
            }
            else {
                levelBodies[y][x] = nullptr;
            }

        }
    }
 
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
    auto blockHeight = calculateBlockHeight();
    auto blockWidth = calculateBlockWidth();

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            if (auto brickPhysics = levelBodies[y][x]) {
                auto brickContacts = brickPhysics->GetContactList();
                if (rand() % 10000 < 10 || (brickContacts && brickContacts->contact->GetFixtureA()->GetBody()->GetLinearVelocity().Length() > 1)) {
                    brickPhysics->SetAwake(true);
                    brickPhysics->SetGravityScale(1);
                }
                auto brickPos = brickPhysics->GetPosition();
                auto brickRot = brickPhysics->GetAngle();

                SDL_Rect srcRect = BRICK_TEX_RECT;
                SDL_Rect blockRect = { static_cast<int>(brickPos.x) - blockWidth / 2, static_cast<int>(brickPos.y) - blockHeight / 2, blockWidth, blockHeight};
                
                SDL_RenderCopyEx(renderer, texture, &srcRect, &blockRect, RADTODEG(brickRot), NULL, SDL_RendererFlip());
            }
        }
    }
}

constexpr int Level::calculateBlockWidth()
{
    return SCREEN_WIDTH / LEVEL_WIDTH;
}

constexpr int Level::calculateBlockHeight()
{
    return (SCREEN_HEIGHT / LEVEL_HEIGHT) * .6f;
}
