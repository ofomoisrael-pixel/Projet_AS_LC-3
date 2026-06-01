#include "game.h"
#include "network.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
static int pieces[7][4][4] =
{
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

void initGame(Game *game)
{
    memset(game->board, 0, sizeof(game->board));

    game->score = 0;
    game->gameOver = 0;

    spawnPiece(game);
}

void spawnPiece(Game *game)
{
    int r = rand() % 7;

    memcpy(
        game->current.shape,
        pieces[r],
        sizeof(pieces[r])
    );

    game->current.x = 3;
    game->current.y = 0;
}

int collision(Game *game, int nx, int ny)
{
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if(game->current.shape[y][x])
            {
                int bx = nx + x;
                int by = ny + y;

                if(bx < 0 || bx >= BOARD_WIDTH)
                    return 1;

                if(by >= BOARD_HEIGHT)
                    return 1;

                if(by >= 0 &&
                   game->board[by][bx])
                    return 1;
            }
        }
    }

    return 0;
}

void movePiece(Game *game, int dx, int dy)
{
    int nx = game->current.x + dx;
    int ny = game->current.y + dy;

    if(!collision(game, nx, ny))
    {
        game->current.x = nx;
        game->current.y = ny;
    }
    else if(dy == 1)
    {
        lockPiece(game);
    }
}

void rotatePiece(Game *game)
{
    int tmp[4][4];

    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            tmp[y][x] =
                game->current.shape[3 - x][y];
        }
    }

    int backup[4][4];

    memcpy(
        backup,
        game->current.shape,
        sizeof(backup)
    );

    memcpy(
        game->current.shape,
        tmp,
        sizeof(tmp)
    );

    if(collision(
        game,
        game->current.x,
        game->current.y))
    {
        memcpy(
            game->current.shape,
            backup,
            sizeof(backup)
        );
    }
}

void hardDrop(Game *game)
{
    while(
        !collision(
            game,
            game->current.x,
            game->current.y + 1))
    {
        game->current.y++;
    }

    lockPiece(game);
}

// ... (garder le début de ton game.c inchangé) ...

void clearLines(Game *game) {
    int lines = 0;
    for(int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        int full = 1;
        for(int x = 0; x < BOARD_WIDTH; x++) {
            if(game->board[y][x] == 0) { full = 0; break; }
        }
        if(full) {
            lines++;
            for(int yy = y; yy > 0; yy--) {
                for(int xx = 0; xx < BOARD_WIDTH; xx++) 
                    game->board[yy][xx] = game->board[yy - 1][xx];
            }
            for(int xx = 0; xx < BOARD_WIDTH; xx++) game->board[0][xx] = 0;
            y++;
        }
    }

    // Gestion des envois de Malus (Attaques) 
    if (lines >= 2) {
        int attackPower = (lines == 4) ? 4 : (lines - 1);
        sendAttack(attackPower); 
        printf("ATTAQUE ENVOYEE : %d lignes !\n", attackPower);
    }
    game->score += lines * 100;
}

// ... (reste du fichier lockPiece et spawnPiece inchangé) ...
void lockPiece(Game *game)
{
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if(game->current.shape[y][x])
            {
                int bx = game->current.x + x;
                int by = game->current.y + y;

                if(by >= 0 &&
                   by < BOARD_HEIGHT &&
                   bx >= 0 &&
                   bx < BOARD_WIDTH)
                {
                    game->board[by][bx] = 1;
                }
            }
        }
    }

    clearLines(game);

    spawnPiece(game);

    if(collision(
        game,
        game->current.x,
        game->current.y))
    {
        game->gameOver = 1;
    }
}

void addGarbage(Game *game, int lines)
{
    for(int i = 0; i < lines; i++)
    {
        for(int y = 0; y < BOARD_HEIGHT - 1; y++)
        {
            for(int x = 0; x < BOARD_WIDTH; x++)
            {
                game->board[y][x] =
                    game->board[y + 1][x];
            }
        }

        int hole = rand() % BOARD_WIDTH;

        for(int x = 0; x < BOARD_WIDTH; x++)
        {
            if(x == hole)
                game->board[BOARD_HEIGHT - 1][x] = 0;
            else
                game->board[BOARD_HEIGHT - 1][x] = 1;
        }
    }
}