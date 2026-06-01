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

    if (!networkInit(ip)) return 1;
    if (!initGraphics()) return 1;

    initGame(&game);
    int running = 1;
    SDL_Event event;
    Uint32 lastFall = SDL_GetTicks();
    Uint32 startTime = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (!game.gameOver && event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  movePiece(&game, -1, 0); break;
                    case SDLK_RIGHT: movePiece(&game, 1, 0);  break;
                    case SDLK_DOWN:  movePiece(&game, 0, 1);  break;
                    case SDLK_UP:    rotatePiece(&game);      break;
                    case SDLK_SPACE: hardDrop(&game);         break;
                }
            }
        }

        receivePackets();
        
        if (!game.gameOver) {
            if (SDL_GetTicks() - lastFall > 500) {
                movePiece(&game, 0, 1);
                lastFall = SDL_GetTicks();
            }
            sendUpdate(&game);
        } else {
            sendGameOver();
        }

        renderGame(&game, (SDL_GetTicks() - startTime) / 1000);
        SDL_Delay(16);
    }

    networkClose();
    cleanupGraphics();
    return 0;
}