#include "BN.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sock-lib.h"
#include "sdl_funcs.h"
#include "client_funcs.h"

#define PORT 8000
#define IP_SERVER "127.0.0.1"

int main(int argc , char** argv) {



    int cliente_fd = conectar(IP_SERVER, PORT, 1);
    if (cliente_fd <= 0){
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }
        struct Game* game = malloc(sizeof(struct Game));

    initializeWindow(game);
    

    
    struct BN_Board* board_self = malloc(2*sizeof(BN_Board*));
    struct BN_Board* board_ = board_self + 1;
    
    //BN_clear_board(board_);

    //BN_change_row(board_, 3, 0b00111000, BN_TYPE_SHIP);

    client_data data = { cliente_fd, game,board_self , board_};


    pthread_t hilo;

    pthread_create(&hilo, NULL, clientLoop, &data);

    while (game->isRunning)
    {
        processInput(game, board_, board_self);
        update(game, board_, board_self);
        render(game, board_, board_self);
    }

    endGame(game);
    freeGame(game);

    return 0;
}