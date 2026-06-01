#ifndef SERVER_H
#define SERVER_H

#define MAX_PLAYERS 4
#define PORT 5000

typedef struct
{
    int id;
    int active;
} Player;

void startServer(void);

#endif