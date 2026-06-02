#include "SDL.h"
#include <time.h>
#include <stdio.h>
#include "game.h"
#include "graphics.h"
#include "network.h"

Game game;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    char ip[16];
    printf("IP Serveur : ");
    scanf("%15s", ip);

    if (!networkInit(ip)) {
        fprintf(stderr, "Erreur réseau : impossible de se connecter au serveur %s\n", ip);
        return 1;
    }
    fprintf(stderr, "networkInit ok\n");

    if (!initGraphics()) {
        fprintf(stderr, "Erreur SDL : impossible d'initialiser l'affichage\n");
        networkClose();
        return 1;
    }
    fprintf(stderr, "initGraphics ok\n");

    initGame(&game);
    fprintf(stderr, "initGame ok\n");
    int running = 1;
    SDL_Event event;
    Uint32 lastFall = SDL_GetTicks();
    Uint32 lastJoin = SDL_GetTicks();
    Uint32 startTime = SDL_GetTicks();
    int gameOverSent = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if ((game.gameOver || game.victory) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                if (mx >= RESTART_BUTTON_X && mx <= RESTART_BUTTON_X + RESTART_BUTTON_W &&
                    my >= RESTART_BUTTON_Y && my <= RESTART_BUTTON_Y + RESTART_BUTTON_H) {
                    resetGame(&game);
                    gameOverSent = 0;
                }
            }

            if (!game.gameOver && !game.victory && event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  movePiece(&game, -1, 0); break;
                    case SDLK_RIGHT: movePiece(&game, 1, 0);  break;
                    case SDLK_DOWN:  movePiece(&game, 0, 1);  break;
                    case SDLK_UP:    rotatePiece(&game);      break;
                    case SDLK_SPACE: hardDrop(&game);         break;
                }
            } else if ((game.gameOver || game.victory) && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                resetGame(&game);
                gameOverSent = 0;
            }
        }

        receivePackets();

        if (myPlayerId == -1 && SDL_GetTicks() - lastJoin > 500) {
            sendJoin();
            lastJoin = SDL_GetTicks();
        }

        checkGameStatus(&game);

        if (!game.gameOver && !game.victory) {
            if (SDL_GetTicks() - lastFall > 500) {
                movePiece(&game, 0, 1);
                lastFall = SDL_GetTicks();
            }
            sendUpdate(&game);
        } else if (game.gameOver && !gameOverSent) {
            sendGameOver();
            gameOverSent = 1;
        }

        renderGame(&game, (SDL_GetTicks() - startTime) / 1000);
        SDL_Delay(16);
    }

    networkClose();
    cleanupGraphics();
    return 0;
}