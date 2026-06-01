#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"
#include "game.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define BLOCK_SIZE 25

extern SDL_Window *window;
extern SDL_Renderer *renderer;

// On garde ta signature de fonction originale
int initGraphics(void);
void renderGame(Game *game, int timeInSeconds);
void cleanupGraphics(void);

#endif