#include "graphics.h"
#include "network.h"
#include <stdio.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initGraphics(void) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    window = SDL_CreateWindow("Tetris Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window) return 0;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return (renderer != NULL);
}

void renderGame(Game *game, int timeInSeconds) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Grille Principale
    for(int y = 0; y < BOARD_HEIGHT; y++) {
        for(int x = 0; x < BOARD_WIDTH; x++) {
            SDL_Rect r = {x * BLOCK_SIZE + 50, y * BLOCK_SIZE + 50, BLOCK_SIZE-1, BLOCK_SIZE-1};
            if(game->board[y][x]) SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
            else SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            SDL_RenderFillRect(renderer, &r);
        }
    }

    // Pièce Actuelle
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(game->current.shape[y][x]) {
                SDL_Rect r = {(game->current.x + x) * BLOCK_SIZE + 50, (game->current.y + y) * BLOCK_SIZE + 50, BLOCK_SIZE-1, BLOCK_SIZE-1};
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }

    // HUD Latéral (Joueurs et Timer)
    SDL_Rect panel = {350, 50, 200, 500};
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &panel);

    for(int i = 0; i < MAX_PLAYERS; i++) {
        SDL_Rect pRect = {370, 80 + (i * 50), 30, 30};
        if (i == myPlayerId) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Toi en blanc
        else if (playersAlive[i]) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Vivant en vert
        else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Mort en rouge
        SDL_RenderFillRect(renderer, &pRect);
    }

    // Barre Timer
    SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
    SDL_Rect tBar = {370, 350, (timeInSeconds * 2) % 160, 20};
    SDL_RenderFillRect(renderer, &tBar);

    if (game->gameOver) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
        SDL_Rect full = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(renderer, &full);
    }

    SDL_RenderPresent(renderer);
}

void cleanupGraphics(void) {
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}