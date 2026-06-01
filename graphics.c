#include "graphics.h"
#include "network.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initGraphics(void) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    window = SDL_CreateWindow("Tetris Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return (renderer != NULL);
}

void renderGame(Game *game, int timeInSeconds) {
    SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
    SDL_RenderClear(renderer);

    for (int p = 0; p < MAX_PLAYERS; p++) {
        int offsetX = p * (10 * BLOCK_SIZE + 20) + 20;

        // Grille de fond
        SDL_Rect area = {offsetX, 50, 10 * BLOCK_SIZE, 20 * BLOCK_SIZE};
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderFillRect(renderer, &area);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 10; x++) {
                int val = (p == myPlayerId) ? game->board[y][x] : otherBoards[p][y][x];
                if (val > 0) {
                    SDL_Rect r = {offsetX + x * BLOCK_SIZE, 50 + y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                    if (p == myPlayerId) SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255);
                    else SDL_SetRenderDrawColor(renderer, 100, 100, 110, 255);
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        // Pièce actuelle (seulement pour toi)
        if (p == myPlayerId && !game->gameOver) {
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    if(game->current.shape[i][j]) {
                        SDL_Rect r = {offsetX + (game->current.x + j) * BLOCK_SIZE, 50 + (game->current.y + i) * BLOCK_SIZE, BLOCK_SIZE-1, BLOCK_SIZE-1};
                        SDL_RenderFillRect(renderer, &r);
                    }
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void cleanupGraphics(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}