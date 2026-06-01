#ifndef GAME_H
#define GAME_H

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

typedef struct {
    int shape[4][4];
    int x, y;
} Piece;

typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    Piece current;
    int gameOver;
    int score;
} Game;

void initGame(Game *game);
int movePiece(Game *game, int dx, int dy);
void rotatePiece(Game *game);
void hardDrop(Game *game);

#endif