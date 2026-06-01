#include "network.h"
#include "game.h"
#include <stdio.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #define ioctlsocket ioctl
#endif

extern Game game;
static int sockfd;
static struct sockaddr_in serverAddr;
int myPlayerId = -1;
int playersAlive[MAX_PLAYERS] = {0, 0, 0, 0};

int networkInit(char *serverIp) {
    #ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);

    unsigned long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);

    Packet join = {PACKET_JOIN, -1, 0, {0}};
    sendto(sockfd, (char*)&join, sizeof(join), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    return 1;
}

void sendUpdate(void) {
    if (myPlayerId == -1 || game.gameOver) return;
    Packet p;
    p.type = PACKET_UPDATE;
    p.playerId = myPlayerId;
    p.value = game.score;
    for(int i=0; i<20; i++) {
        p.boardState[i] = 0;
        for(int j=0; j<10; j++) if(game.board[i][j]) p.boardState[i] = 1;
    }
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void receivePackets(void) {
    Packet p;
    struct sockaddr_in from;
    int len = sizeof(from);
    while (recvfrom(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&from, &len) > 0) {
        if (p.type == PACKET_JOIN) myPlayerId = p.playerId;
        if (p.type == PACKET_UPDATE) {
            if (p.playerId >= 0 && p.playerId < 4) playersAlive[p.playerId] = 1;
        }
        if (p.type == PACKET_GAMEOVER) {
            if (p.playerId >= 0 && p.playerId < 4) playersAlive[p.playerId] = 0;
        }
    }
}

void sendAttack(int lines) {
    Packet p = {PACKET_ATTACK, myPlayerId, lines, {0}};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void sendGameOver(void) {
    Packet p = {PACKET_GAMEOVER, myPlayerId, 0, {0}};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void networkClose(void) {
    #ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
    #else
    close(sockfd);
    #endif
}