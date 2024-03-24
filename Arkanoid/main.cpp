#include<iostream>
#include "SDL.h"
#include "Level.h"
#include "Physics.h"


int main(int argc, char* argv[])
{    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        return 1;
    }

    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Physics physics;
    physics.init();
    Level level;
    
    if (level.init(renderer, physics)) {

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

            if (elapsedTime < TARGET_FRAME_TIME) {
                SDL_Delay(TARGET_FRAME_TIME - elapsedTime);
            }

            // Set the background color (optional)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black

            // Clear the window
            SDL_RenderClear(renderer);

            physics.update();
            level.draw(renderer, physics);

            // Update the screen
            SDL_RenderPresent(renderer);
        }
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}


