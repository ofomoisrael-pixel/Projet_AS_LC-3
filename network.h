#ifndef NETWORK_H
#define NETWORK_H

#define SERVER_PORT 5000
#define MAX_PLAYERS 4

typedef struct {
    int type;
    int playerId;
    int value;
    int boardState[20]; 
} Packet;

enum {
    PACKET_JOIN,
    PACKET_UPDATE,
    PACKET_ATTACK,
    PACKET_GAMEOVER,
    PACKET_WINNER
};

int networkInit(char *serverIp);
void sendUpdate(void);
void sendAttack(int lines);
void sendGameOver(void);
void receivePackets(void);
void networkClose(void);

extern int myPlayerId;
extern int playersAlive[MAX_PLAYERS];

#endif