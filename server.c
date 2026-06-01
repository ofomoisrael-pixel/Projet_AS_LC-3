#include "server.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t; // Correction cruciale pour Windows
#else
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include <stdio.h>
#include <string.h>

#define MAX_PLAYERS 4

typedef struct {
    struct sockaddr_in addr;
    int active;
} Client;

typedef struct {
    int type;
    int playerId;
    int value;
} Packet;

enum {
    PACKET_JOIN,
    PACKET_ATTACK,
    PACKET_GAMEOVER,
    PACKET_WINNER
};

Client clients[MAX_PLAYERS];

int main() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    int sockfd;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Erreur de bind\n");
        return 1;
    }

    printf("Serveur Tetris Battle lance sur le port 5000...\n");

    int playerCount = 0;
    Packet packet;
    struct sockaddr_in sender;
    socklen_t len = sizeof(sender);

    while(1) {
        // Ajout du cast (char*) pour éviter les warnings
        int n = recvfrom(sockfd, (char*)&packet, sizeof(packet), 0, (struct sockaddr*)&sender, &len);
        
        if (n > 0) {
            if (packet.type == PACKET_JOIN) {
                if (playerCount < MAX_PLAYERS) {
                    clients[playerCount].addr = sender;
                    clients[playerCount].active = 1;
                    
                    Packet response = {PACKET_JOIN, playerCount, 0};
                    sendto(sockfd, (char*)&response, sizeof(response), 0, (struct sockaddr*)&sender, len);
                    
                    printf("Joueur %d connecte depuis %s\n", playerCount, inet_ntoa(sender.sin_addr));
                    playerCount++;
                }
            }

            if (packet.type == PACKET_ATTACK) {
                for (int i = 0; i < playerCount; i++) {
                    if (i != packet.playerId && clients[i].active) {
                        sendto(sockfd, (char*)&packet, sizeof(packet), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                    }
                }
            }

            if (packet.type == PACKET_GAMEOVER) {
                if(packet.playerId < playerCount) clients[packet.playerId].active = 0;
                
                int aliveCount = 0;
                int winnerId = -1;
                for (int i = 0; i < playerCount; i++) {
                    if (clients[i].active) {
                        aliveCount++;
                        winnerId = i;
                    }
                }

                if (aliveCount == 1) {
                    Packet win = {PACKET_WINNER, winnerId, 0};
                    for (int i = 0; i < playerCount; i++) {
                        sendto(sockfd, (char*)&win, sizeof(win), 0, (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                    }
                }
            }
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}