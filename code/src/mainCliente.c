#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sock-lib.h"

#define PORT 8000
#define IP_SERVER "127.0.0.1"

int main(int argc , char** argv) {

    int tipo_solicitado = 1;

    int cliente_fd = conectar(IP_SERVER, PORT, 1);
    if (cliente_fd < 0) {
        perror("Error al conectar con el servidor");
        exit(EXIT_FAILURE);
    }

    if(argc > 1)
        tipo_solicitado = atoi(*(argv+1));

    write(cliente_fd, &tipo_solicitado, sizeof(int));

    recibir_respuesta(cliente_fd);

    close(cliente_fd);
    return 0;
}