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
    int victory; // 0 = en cours/perdu, 1 = gagné
} Game;

// Fonctions d'initialisation et de cycle de vie
void initGame(Game *game);
void spawnPiece(Game *game);
void checkGameStatus(Game *game);

// Fonctions de mouvements du joueur
void movePiece(Game *game, int dx, int dy);
void rotatePiece(Game *game);
void hardDrop(Game *game);

// Fonctions logiques internes du jeu
void lockPiece(Game *game);
void clearLines(Game *game);
int collision(Game *game, int x, int y);

// Fonction d'interaction multijoueur (Malus)
void addGarbage(Game *game, int lines);

#endif