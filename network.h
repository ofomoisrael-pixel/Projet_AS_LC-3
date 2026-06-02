#ifndef NETWORK_H
#define NETWORK_H

#include "game.h"

#define MAX_PLAYERS 4
#define SERVER_PORT 5000

typedef struct {
    int type;
    int playerId;
    int value;
    int boardState[20][10]; 
} Packet;

enum { PACKET_JOIN, PACKET_UPDATE, PACKET_ATTACK, PACKET_GAMEOVER };

int networkInit(char *serverIp);
void sendUpdate(Game *game);
void sendJoin(void);
void receivePackets(void);
void sendAttack(int lines);
void sendGameOver(void);
void networkClose(void);

extern int myPlayerId;
extern int otherBoards[MAX_PLAYERS][20][10];
extern int playersActive[MAX_PLAYERS];

#endif