#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h> // Indispensable pour inet_pton
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif
#include "SDL.h" // Ou "SDL2/SDL.h" selon ton dossier
#include <stdio.h>
#include "network.h"
#include "game.h"

#pragma comment(lib, "ws2_32.lib")

extern Game game;
static int sockfd;
static struct sockaddr_in serverAddr;
int myPlayerId = -1;

int networkInit(char *serverIp) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);

    // Socket non-bloquant
    unsigned long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);

    Packet join = {PACKET_JOIN, 0, 0};
    sendto(sockfd, (char*)&join, sizeof(join), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    return 1;
}

void receivePackets(void) {
    Packet p;
    struct sockaddr_in from;
    int len = sizeof(from);
    while (recvfrom(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&from, &len) > 0) {
        if (p.type == PACKET_ATTACK) addGarbage(&game, p.value);
        if (p.type == PACKET_JOIN) myPlayerId = p.playerId;
    }
}

void sendAttack(int lines) {
    Packet p = {PACKET_ATTACK, myPlayerId, lines};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void sendGameOver(void) {
    Packet p = {PACKET_GAMEOVER, myPlayerId, 0};
    sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}