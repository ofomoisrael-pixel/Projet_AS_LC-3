#ifndef NETWORK_H
#define NETWORK_H

// On inclut game.h car on utilise le type "Game" plus bas
#include "game.h"

#define MAX_PLAYERS 4
#define SERVER_PORT 5000

typedef struct {
    int type;
    int playerId;
    int value;
    int boardState[20][10]; // La grille complète pour l'affichage multi
} Packet;

// Types de paquets
enum { 
    PACKET_JOIN, 
    PACKET_UPDATE, 
    PACKET_ATTACK, 
    PACKET_GAMEOVER 
};

// Fonctions réseau
int networkInit(char *serverIp);
void sendUpdate(Game *game);
void receivePackets(void);
void networkClose(void); // Important pour bien fermer le socket

// Variables globales partagées
extern int myPlayerId;
extern int otherBoards[MAX_PLAYERS][20][10]; 
extern int playersActive[MAX_PLAYERS];

#endif