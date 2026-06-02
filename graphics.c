#include <stdio.h>
#include "graphics.h"
#include "network.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initGraphics(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Tetris Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

void renderGame(Game *game, int timeInSeconds) {
    SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
    SDL_RenderClear(renderer);

    int offsetX = 20;
    int offsetY = 50;

    // Grille de fond
    SDL_Rect area = {offsetX, offsetY, 10 * BLOCK_SIZE, 20 * BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
    SDL_RenderFillRect(renderer, &area);
    SDL_SetRenderDrawColor(renderer, 80, 80, 90, 255);
    SDL_RenderDrawRect(renderer, &area);

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (game->board[y][x]) {
                SDL_Rect r = {offsetX + x * BLOCK_SIZE, offsetY + y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255);
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }

    if (!game->gameOver && !game->victory) {
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (game->current.shape[i][j]) {
                    SDL_Rect r = {offsetX + (game->current.x + j) * BLOCK_SIZE, offsetY + (game->current.y + i) * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
    }

    if (game->gameOver || game->victory) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_Rect overlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &overlay);

        SDL_SetRenderDrawColor(renderer, game->victory ? 0 : 255, game->victory ? 255 : 0, 0, 220);
        SDL_Rect statusBox = {30, 160, WINDOW_WIDTH - 60, 80};
        SDL_RenderFillRect(renderer, &statusBox);

        SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);
        SDL_Rect button = {RESTART_BUTTON_X, RESTART_BUTTON_Y, RESTART_BUTTON_W, RESTART_BUTTON_H};
        SDL_RenderFillRect(renderer, &button);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &button);

        // Simple label en forme de R sur le bouton
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect r1 = {button.x + 20, button.y + 10, 10, 20};
        SDL_Rect r2 = {button.x + 30, button.y + 10, 18, 6};
        SDL_Rect r3 = {button.x + 30, button.y + 20, 18, 6};
        SDL_Rect r4 = {button.x + 42, button.y + 16, 6, 10};
        SDL_RenderFillRect(renderer, &r1);
        SDL_RenderFillRect(renderer, &r2);
        SDL_RenderFillRect(renderer, &r3);
        SDL_RenderFillRect(renderer, &r4);
    }

    SDL_RenderPresent(renderer);
}

void cleanupGraphics(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}