#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SDL.h"
#include "game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 700
#define BLOCK_SIZE 30

extern SDL_Window *window;
extern SDL_Renderer *renderer;

int initGraphics(void);

void renderGame(Game *game);

void cleanupGraphics(void);

#endif