#include "SDL.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "graphics.h"
#include "network.h"

Game game;

void afficherTuto() {
    printf("\n=== TUTORIEL TETRIS BATTLE ===\n");
    printf("- FLECHES : Deplacer / Tourner\n");
    printf("- ESPACE  : Chute rapide\n");
    printf("==============================\n\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    char ip[16];

    printf("Entrez l'IP du serveur : ");
    if (scanf("%15s", ip) != 1) return 1;

    if (!networkInit(ip)) {
        printf("Erreur de connexion reseau.\n");
        return 1;
    }

    if (!initGraphics()) return 1;

    afficherTuto();
    initGame(&game);
    
    int running = 1;
    SDL_Event event;
    Uint32 lastFall = SDL_GetTicks();
    Uint32 startTime = SDL_GetTicks(); // Debut du chrono

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

        int timeInSeconds = (SDL_GetTicks() - startTime) / 1000;

        if (!game.gameOver) {
            Uint32 now = SDL_GetTicks();
            if (now - lastFall > 500) {
                movePiece(&game, 0, 1);
                lastFall = now;
            }
        }
        
        renderGame(&game, timeInSeconds);
        SDL_Delay(16);
    }
    
    cleanupGraphics();
    return 0;
}