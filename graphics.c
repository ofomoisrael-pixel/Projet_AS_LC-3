#include "graphics.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initGraphics(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 0;

    window = SDL_CreateWindow(
        "Tetris Battle",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(!window)
        return 0;

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(!renderer)
        return 0;

    return 1;
}

static void drawBoard(Game *game)
{
    SDL_Rect rect;

    rect.w = BLOCK_SIZE;
    rect.h = BLOCK_SIZE;

    for(int y = 0; y < BOARD_HEIGHT; y++)
    {
        for(int x = 0; x < BOARD_WIDTH; x++)
        {
            if(game->board[y][x])
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    0,
                    200,
                    255,
                    255
                );
            }
            else
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    30,
                    30,
                    30,
                    255
                );
            }

            rect.x = x * BLOCK_SIZE + 50;
            rect.y = y * BLOCK_SIZE + 50;

            SDL_RenderFillRect(renderer, &rect);

            SDL_SetRenderDrawColor(
                renderer,
                0,
                0,
                0,
                255
            );

            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

static void drawCurrentPiece(Game *game)
{
    SDL_Rect rect;

    rect.w = BLOCK_SIZE;
    rect.h = BLOCK_SIZE;

    SDL_SetRenderDrawColor(
        renderer,
        255,
        200,
        0,
        255
    );

    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if(game->current.shape[y][x])
            {
                rect.x =
                    (game->current.x + x)
                    * BLOCK_SIZE + 50;

                rect.y =
                    (game->current.y + y)
                    * BLOCK_SIZE + 50;

                SDL_RenderFillRect(
                    renderer,
                    &rect
                );

                SDL_SetRenderDrawColor(
                    renderer,
                    0,
                    0,
                    0,
                    255
                );

                SDL_RenderDrawRect(
                    renderer,
                    &rect
                );

                SDL_SetRenderDrawColor(
                    renderer,
                    255,
                    200,
                    0,
                    255
                );
            }
        }
    }
}

// ... (garder tes fonctions init et drawBoard) ...

void renderGame(Game *game) {
    // Si game over, on assombrit l'écran
    if (game->gameOver) {
        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    }

    SDL_RenderClear(renderer);
    drawBoard(game);
    drawCurrentPiece(game);

    // Petit indicateur de score
    SDL_RenderPresent(renderer);
}

void cleanupGraphics(void)
{
    if(renderer)
        SDL_DestroyRenderer(renderer);

    if(window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}