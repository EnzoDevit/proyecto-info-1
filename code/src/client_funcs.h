#include "sdl_funcs.h"
#include "sock-lib.h"

//struct de datos para pasarle al thrad creado por el cliente
typedef struct client_data{
    int sock_descriptor;
    Game* game;
    BN_Board* board1;
    BN_Board* board2;
}client_data;

static int shipWalk(BN_Board* board,  int x,  int y, char is_x, char dir);
void BN_getShip(BN_Board* board, Node** list ,unsigned int x, unsigned int y);
void BN_processResponse(Game* game, BN_Board* board, unsigned char x, unsigned char y, unsigned char statustype);
void* clientLoop(void* data);