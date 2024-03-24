#include "Level.h"
#include "SDL.h"
#include "box2d/box2d.h" //TODO:REMOVE PHYSICS IN THIS FILE
#include "Physics.h"

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
                levelBodies[y][x] = physics.addBrick(x * blockWidth + blockWidth / 2, y * blockHeight + blockHeight / 2, calculateBlockWidth(), calculateBlockHeight(), true, true);
            }
            else {
                levelBodies[y][x] = nullptr;
            }

        }
    }
 
    testBody = physics.addBrick(testX, testY, 50, calculateBlockHeight(), false, false);

    SDL_Surface* surface = SDL_LoadBMP("SNES_ Arkanoid.BMP");
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
                auto brickPos = brickPhysics->GetPosition();
                auto brickRot = brickPhysics->GetAngle();

                SDL_Rect srcRect = { 158, 83, 15, 7 };
                SDL_Rect blockRect = { static_cast<int>(brickPos.x) - blockWidth / 2, static_cast<int>(brickPos.y) - blockHeight / 2, blockWidth, blockHeight};
                
                SDL_RenderCopyEx(renderer, texture, &srcRect, &blockRect, RADTODEG(brickRot), NULL, SDL_RendererFlip());
            }
        }
    }
    bool awake = testBody->IsAwake();
    auto pos = testBody->GetPosition();

    testX = static_cast<int>(pos.x) - 25;
    testY = static_cast<int>(pos.y) - blockHeight / 2;
    
    SDL_Rect blockRect = { testX, testY, 50, blockHeight };
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &blockRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &blockRect);
}

constexpr int Level::calculateBlockWidth()
{
    return SCREEN_WIDTH / LEVEL_WIDTH;
}

constexpr int Level::calculateBlockHeight()
{
    return (SCREEN_HEIGHT / LEVEL_HEIGHT) * .6f;
}
