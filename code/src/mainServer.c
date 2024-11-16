#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sock-lib.h"

#define PORT 8000


int main(){

    int servidor_fd = abrir_conexion(PORT, 10, 1);
    if (servidor_fd < 0) {
        perror("Error al abrir conexión");
        exit(EXIT_FAILURE);
    }



    while (1) {
        int *nuevo_cliente_fd = malloc(sizeof(int));
        *nuevo_cliente_fd = aceptar_pedidos(servidor_fd, 1);
            if (*nuevo_cliente_fd < 0){
                perror("Error al aceptar conexión");
                free(nuevo_cliente_fd);
                continue;
            }
    }
}