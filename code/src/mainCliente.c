#include "client_funcs.h"

#define PORT 8000
#define IP_LOCAL "127.0.0.1"

//FREE DE LAS VARIABLES board_self despues de que se termine el juego

int main(int argc, char** argv){
    
    char* ip_server = (argc>1)?*(argv+1):IP_LOCAL;
    //se conecta a la ip
    int cliente_fd = conectar(IP_LOCAL, PORT, 1);

    if (cliente_fd <= 0){
        printf("Error al conectar con el servidor");
    }
    else {
        Game* game = (Game*)malloc(sizeof(Game));

        if(initializeGame(game) == 0)
        {
            game->sd = cliente_fd;

            struct BN_Board* board_self = (BN_Board*)malloc(sizeof(BN_Board*));
            struct BN_Board* board_ = (BN_Board*)malloc(sizeof(BN_Board*));

            client_data data = { cliente_fd, game,board_self , board_};

            pthread_t hilo;
            /* */
            pthread_create(&hilo, NULL, clientLoop, &data);

            while (game->isRunning) {
                processInput(game, board_, board_self);
                update(game, board_, board_self);
                render(game, board_, board_self);
            }

            if(!(game->threadEnded)) {
                printf("llega\n"); fflush(stdout);
                pthread_join(hilo, NULL);
            }
            // */
            close(cliente_fd);

            printf("endGame\n"); fflush(stdout);
            endGame(game);
            
            freeGame(game);
            printf("free(board_self)\n"); fflush(stdout);
            free(board_self);
            free(board_);
        }
    }
    return 0;
}