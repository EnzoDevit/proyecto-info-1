#include "client_funcs.h"

#define PORT 8000
#define IP_LOCAL "127.0.0.1"

//FREE DE LAS VARIABLES board_self despues de que se termine el juego

int main(int argc, char** argv){
    

    char* ip_server = IP_LOCAL;
    if(argc > 1) ip_server = *(argv + 1);

    int cliente_fd = conectar(ip_server, PORT, 0);

    //si hay algún problema con la conexión
    if (cliente_fd <= 0){
        printf("Error al conectar con el servidor");
    }
    else {

        // La struct que usamos para pasar la
        // informacion entre funciones y threads
        Game* game = (Game*)malloc(sizeof(Game));

        // Si se logra inicializar
        if(initializeGame(game) == 0)
        {
            game->sd = cliente_fd;

            struct BN_Board* board_self = (BN_Board*)malloc(sizeof(BN_Board*));
            struct BN_Board* board_ = (BN_Board*)malloc(sizeof(BN_Board*));

            // Data a pasarle al thread
            client_data data = { cliente_fd, game,board_self , board_};

            pthread_t hilo;
            pthread_create(&hilo, NULL, clientLoop, &data);

            while (game->isRunning) {
                processInput(game, board_, board_self);
                render(game, board_, board_self);
            }
            // Por si se queda en el read, lo espero
            if(!(game->threadEnded)) {
                pthread_join(hilo, NULL);
            }
            close(cliente_fd);

            endGame(game);

            // El main es el responsable de liberar la memoria
            freeGame(game);
            free(board_self);
            free(board_);
        }
    }
    return 0;
}