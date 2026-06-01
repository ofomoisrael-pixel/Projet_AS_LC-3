#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"
#include "game.h"

// 4 grilles de 10 colonnes * 20 pixels = 800px + marges
#define WINDOW_WIDTH 900 
#define WINDOW_HEIGHT 600
#define BLOCK_SIZE 20 // On réduit un peu la taille pour que tout tienne

extern SDL_Window *window;
extern SDL_Renderer *renderer;

int initGraphics(void);
void renderGame(Game *game, int timeInSeconds);
void cleanupGraphics(void);

#endif