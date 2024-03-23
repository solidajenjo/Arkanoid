#include "Level.h"
#include "SDL.h"

Level::Level()
{
    const char* levelRows[] = {
        "00000000000000000000",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
        "01111111111111111110",
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
        }
    }
}

void Level::draw(SDL_Renderer* renderer)
{

    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
        for (int x = 0; x < LEVEL_WIDTH; ++x) {
            if (levelData[y][x] != '0') {
                auto blockHeight = calculateBlockHeight();
                auto blockWidth = calculateBlockWidth();
                SDL_Rect blockRect = {x * blockWidth, y * blockHeight, blockWidth, blockHeight};
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

                SDL_RenderFillRect(renderer, &blockRect);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                SDL_RenderDrawRect(renderer, &blockRect);
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
    return SCREEN_HEIGHT / LEVEL_HEIGHT;
}
