#include<iostream>
#include "SDL.h"

int main()
{    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Get screen dimensions
    int screenWidth, screenHeight;
    SDL_GetWindowSize(NULL, &screenWidth, &screenHeight);

    // Calculate rectangle position for centering
    int rectX = (screenWidth - 10) / 2;
    int rectY = (screenHeight - 10) / 2;

    // Create a window
    SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
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

    // Set the background color (optional)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black

    // Clear the window
    SDL_RenderClear(renderer);

    // Set the rectangle color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white

    // Fill the rectangle
    SDL_Rect rect = { rectX, rectY, 10, 10 };
    SDL_RenderFillRect(renderer, &rect);

    // Update the screen
    SDL_RenderPresent(renderer);

    // Wait for a key press to close the window
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    // Clean up resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}


