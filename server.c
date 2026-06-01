#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5000
#define MAX_CLIENTS 4

typedef struct {
    int type;
    int playerId;
    int value;
    int boardState[20];
} Packet;

struct sockaddr_in clients[MAX_CLIENTS];
int clientActive[MAX_CLIENTS] = {0};

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("SERVEUR TETRIS LANCER SUR LE PORT %d...\n", PORT);

    Packet p;
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);

    while (1) {
        int res = recvfrom(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (res > 0) {
            // NOUVEAU JOUEUR
            if (p.type == 0) { // PACKET_JOIN
                int id = -1;
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!clientActive[i]) {
                        clients[i] = clientAddr;
                        clientActive[i] = 1;
                        id = i;
                        break;
                    }
                }
                p.playerId = id;
                sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&clientAddr, addrLen);
                printf("Joueur %d connecte !\n", id);
            } 
            // TRANSMISSION AUX AUTRES
            else {
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clientActive[i]) {
                        sendto(sockfd, (char*)&p, sizeof(p), 0, (struct sockaddr*)&clients[i], addrLen);
                    }
                }
            }
        }
    }
    closesocket(sockfd);
    WSACleanup();
    return 0;
}