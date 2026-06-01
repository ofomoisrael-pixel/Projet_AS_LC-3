#include "graphics.h"
#include <stdio.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initGraphics(void) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    window = SDL_CreateWindow("Tetris Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window) return 0;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return (renderer != NULL);
}

static void drawHUD(Game *game, int timeInSeconds) {
    // Panneau latéral
    SDL_Rect sidePanel = {BOARD_WIDTH * BLOCK_SIZE + 70, 50, 180, 400};
    SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
    SDL_RenderFillRect(renderer, &sidePanel);

    // Liste des joueurs (Indicateurs)
    for(int i = 0; i < 4; i++) {
        SDL_Rect pRect = {BOARD_WIDTH * BLOCK_SIZE + 90, 80 + (i * 45), 20, 20};
        if (i == 0 && !game->gameOver) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Toi vivant
        else if (i == 0 && game->gameOver) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Toi mort
        else SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Autres
        SDL_RenderFillRect(renderer, &pRect);
    }

    // Barre de Temps
    SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
    SDL_Rect timeBar = {BOARD_WIDTH * BLOCK_SIZE + 90, 350, (timeInSeconds % 150), 15};
    SDL_RenderFillRect(renderer, &timeBar);
}

void renderGame(Game *game, int timeInSeconds) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Dessin du plateau (board)
    for(int y = 0; y < BOARD_HEIGHT; y++) {
        for(int x = 0; x < BOARD_WIDTH; x++) {
            SDL_Rect r = {x * BLOCK_SIZE + 50, y * BLOCK_SIZE + 50, BLOCK_SIZE-1, BLOCK_SIZE-1};
            if(game->board[y][x]) SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
            else SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
            SDL_RenderFillRect(renderer, &r);
        }
    }

    // Dessin de la pièce actuelle
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
    for(int y = 0; y < 4; y++) {
        for(int x = 0; x < 4; x++) {
            if(game->current.shape[y][x]) {
                SDL_Rect r = {(game->current.x + x) * BLOCK_SIZE + 50, (game->current.y + y) * BLOCK_SIZE + 50, BLOCK_SIZE-1, BLOCK_SIZE-1};
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }

    drawHUD(game, timeInSeconds);

    if (game->gameOver) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 120);
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