#include <pthread.h>
#include "sock-lib.h"
#include "client_funcs.h"

#define PORT 8000
#define IP_SERVER "127.0.0.1"

//FREE DE LAS VARIABLES board_self despues de que se termine el juego

int main(){

    int cliente_fd = conectar(IP_SERVER, PORT, 1);

    if (cliente_fd <= 0){
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }
    
    struct Game* game = (Game*)malloc(sizeof(struct Game));

    initializeGame(game);
    
    game->sd = cliente_fd;

    struct BN_Board* board_self = (BN_Board*)malloc(2*sizeof(BN_Board*));
    struct BN_Board* board_ = board_self + 1;
    
    //BN_clear_board(board_);

    //BN_change_row(board_, 3, 0b00111000, BN_TYPE_SHIP);

    client_data data = { cliente_fd, game,board_self , board_};


    pthread_t hilo;

    pthread_create(&hilo, NULL, clientLoop, &data);
    //pthread_detach(hilo);

    while (game->isRunning)
    {
        processInput(game, board_, board_self);
        update(game, board_, board_self);
        render(game, board_, board_self);
    }

    if(!(game->threadEnded))
    {
        printf("llega\n"); fflush(stdout);
        //pthread_kill(hilo, SIGTERM);
        pthread_join(hilo, NULL);
    }

    close(cliente_fd);

    
    printf("endGame\n"); fflush(stdout);
    endGame(game);
    
    freeGame(game);
    free(board_self);

    return 0;
}