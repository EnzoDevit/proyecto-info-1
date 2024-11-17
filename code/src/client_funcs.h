#include "BN.h"
#include "sdl_funcs.h"

typedef struct client_data{
    int sock_descriptor;
    Game* game;
    BN_Board* board1;
    BN_Board* board2;
}client_data;

void BN_processResponse(Game* game, BN_Board* board, unsigned char x, unsigned char y, unsigned char statustype);
void* clientLoop(void* data);