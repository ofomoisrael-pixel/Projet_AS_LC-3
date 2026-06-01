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
    
    // Demande l'IP avant d'ouvrir la fenêtre SDL
    printf("IP Serveur : ");
    if (scanf("%15s", ip) != 1) return 1;

    // Initialisation du réseau et des graphismes
    if (!networkInit(ip)) {
        printf("Erreur d'initialisation reseau.\n");
        return 1;
    }
    if (!initGraphics()) {
        printf("Erreur d'initialisation graphique.\n");
        return 1;
    }

    initGame(&game);
    int running = 1;
    SDL_Event event;
    Uint32 lastFall = SDL_GetTicks();
    Uint32 startTime = SDL_GetTicks();

    while (running) {
        // 1. Gestion des entrées (Clavier)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (!game.gameOver && event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:  movePiece(&game, -1, 0); break;
                    case SDLK_RIGHT: movePiece(&game, 1, 0);  break;
                    case SDLK_DOWN:  movePiece(&game, 0, 1);  break;
                    case SDLK_UP:    rotatePiece(&game);      break;
                    case SDLK_SPACE: hardDrop(&game);         break;
                    case SDLK_ESCAPE: running = 0;            break;
                }
            }
        }

        // 2. Réception des données des autres joueurs
        receivePackets();
        
        int timePassed = (SDL_GetTicks() - startTime) / 1000;

        // 3. Logique du jeu et Envoi réseau
        if (!game.gameOver) {
            // Chute automatique de la pièce
            if (SDL_GetTicks() - lastFall > 500) {
                movePiece(&game, 0, 1);
                lastFall = SDL_GetTicks();
            }
            
            // CORRECTIF : On passe &game en argument comme défini dans network.h
            sendUpdate(&game); 
        } else {
            // Signal de défaite envoyé une seule fois ou en boucle au serveur
            sendGameOver();
        }

        // 4. Rendu graphique des 4 colonnes
        renderGame(&game, timePassed);

        // Limitation à ~60 FPS
        SDL_Delay(16);
    }

    // Nettoyage avant de quitter
    networkClose();
    cleanupGraphics();
    
    return 0;
}