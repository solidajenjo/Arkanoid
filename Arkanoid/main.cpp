#include<iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Level.h"
#include "Physics.h"
#include <string>
#include "main.h"
#include "Ball.h"


int main(int argc, char* argv[])
{    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0)  {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Create a window
    SDL_Window* window = SDL_CreateWindow("Almost physically accurate Arkanoid", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("Retro_Gaming.ttf", 20); 
    if (font == nullptr) {
        // Handle error
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Physics physics;
    physics.init();
    Level level;
    
    if (level.init(renderer, physics)) {

        Ball ball;
        ball.init(physics);

        int lastFrameTime = SDL_GetTicks(); // Get starting time

        // Wait for a key press to close the window
        SDL_Event event;
        bool quit = false;
        while (!quit) {
            
            int currentFrameTime = SDL_GetTicks(); // Get current time
            int elapsedTime = currentFrameTime - lastFrameTime; // Time since last frame

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = true;
                }
            }
            
            while (elapsedTime < TARGET_FRAME_TIME) {
                elapsedTime = SDL_GetTicks() - lastFrameTime;
                //physics.update();
                SDL_Delay(1);
            } 
            
            lastFrameTime = SDL_GetTicks();

            // Set the background color (optional)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black

            // Clear the window
            SDL_RenderClear(renderer);

            ball.update(renderer, physics);
            physics.update();
            level.draw(renderer, physics);
#if 1
            physics.debugDraw(renderer);
#endif
            OnRenderUI(elapsedTime, font, renderer);

            // Update the screen
            SDL_RenderPresent(renderer);
        }
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}

void OnRenderUI(int elapsedTime, TTF_Font* font, SDL_Renderer* renderer)
{
    SDL_Color textColor = UI_COLOR;
    std::string msg = "Frame time: " + std::to_string(elapsedTime) + " FPS:" + std::to_string(static_cast<int>(1.f / static_cast<float>(elapsedTime) * 1000.f));
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, msg.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = 10;
    textRect.y = 10;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}


