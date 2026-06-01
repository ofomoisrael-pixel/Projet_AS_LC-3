#include "graphics.h"
#include "network.h"

void renderGame(Game *game, int timeInSeconds) {
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);

    for (int p = 0; p < MAX_PLAYERS; p++) {
        int offsetX = p * 220 + 20; // Décalage pour chaque joueur

        // Dessin du cadre de la colonne
        SDL_Rect border = {offsetX - 2, 48, (10 * BLOCK_SIZE) + 4, (20 * BLOCK_SIZE) + 4};
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &border);

        // Dessin de la grille (soit la tienne, soit celle reçue par le réseau)
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 10; x++) {
                int blockValue;
                if (p == myPlayerId) blockValue = game->board[y][x];
                else blockValue = otherBoards[p][y][x];

                SDL_Rect r = {offsetX + (x * BLOCK_SIZE), 50 + (y * BLOCK_SIZE), BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                
                if (blockValue > 0) {
                    if (p == myPlayerId) SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255); // Toi en bleu
                    else SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); // Les autres en gris
                } else {
                    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Vide
                }
                SDL_RenderFillRect(renderer, &r);
            }
        }

        // Si c'est toi, on dessine aussi ta pièce qui tombe
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

        // Affichage "GAME OVER" sur la colonne si le joueur a perdu
        if (!playersActive[p] && p < MAX_PLAYERS) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            SDL_RenderFillRect(renderer, &border);
        }
    }

    // Timer en bas
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect tBar = {20, 500, timeInSeconds % 860, 10};
    SDL_RenderFillRect(renderer, &tBar);

    SDL_RenderPresent(renderer);
}