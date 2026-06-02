#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"
#include "game.h"

// 1 grille de 10 colonnes * 20 pixels = 200px + marges
#define WINDOW_WIDTH 260
#define WINDOW_HEIGHT 500
#define BLOCK_SIZE 20 // Taille des blocs du plateau

#define RESTART_BUTTON_X 60
#define RESTART_BUTTON_Y 360
#define RESTART_BUTTON_W 140
#define RESTART_BUTTON_H 40

extern SDL_Window *window;
extern SDL_Renderer *renderer;

int initGraphics(void);
void renderGame(Game *game, int timeInSeconds);
void cleanupGraphics(void);

#endif