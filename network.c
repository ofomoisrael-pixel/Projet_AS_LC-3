#include "network.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #define ioctlsocket ioctl
#endif

// Définition des variables globales (ce qui manquait pour le linker)
int myPlayerId = -1;
int otherBoards[MAX_PLAYERS][20][10] = {0};
int playersActive[MAX_PLAYERS] = {0};

static SOCKET sockfd;
static struct sockaddr_in serverAddr;

int networkInit(char *serverIp) {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;
#endif

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) return 0;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);

    unsigned long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);

    Packet p = {PACKET_JOIN, -1, 0, {{0}}};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    return 1;
}

void sendUpdate(Game *game) {
    if (myPlayerId == -1) return;
    Packet p;
    p.type = PACKET_UPDATE;
    p.playerId = myPlayerId;
    p.value = game->score;
    memcpy(p.boardState, game->board, sizeof(game->board));
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void sendAttack(int lines) {
    Packet p = {PACKET_ATTACK, myPlayerId, lines, {{0}}};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void sendGameOver(void) {
    Packet p = {PACKET_GAMEOVER, myPlayerId, 0, {{0}}};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void receivePackets(void) {
    Packet p;
    struct sockaddr_in from;
    int len = sizeof(from);
    while (recvfrom(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&from, &len) > 0) {
        if (p.type == PACKET_JOIN && myPlayerId == -1) {
            myPlayerId = p.playerId;
        }
        if (p.playerId >= 0 && p.playerId < MAX_PLAYERS) {
            playersActive[p.playerId] = 1;
            if (p.type == PACKET_UPDATE) {
                memcpy(otherBoards[p.playerId], p.boardState, sizeof(p.boardState));
            } else if (p.type == PACKET_GAMEOVER) {
                playersActive[p.playerId] = 0;
            }
        }
    }
}

void networkClose(void) {
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
}