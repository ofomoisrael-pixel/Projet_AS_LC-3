#ifndef GAME_H
#define GAME_H

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct
{
    int x;
    int y;
    int shape[4][4];
} Piece;

typedef struct
{
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    Piece current;
    int score;
    int gameOver;
} Game;

void initGame(Game *game);

void spawnPiece(Game *game);

void movePiece(Game *game,int dx,int dy);

void rotatePiece(Game *game);

void hardDrop(Game *game);

void lockPiece(Game *game);

void clearLines(Game *game);

void addGarbage(Game *game,int lines);

int collision(Game *game,int x,int y);

#endif