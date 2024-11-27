#include "server_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sock-lib.h"
#include <time.h>

#define PORT 8000


int main(){
    // Setea la seed para los numeros random de los tableros
    srand(time(NULL));
    int servidor_fd = abrir_conexion(PORT, 10, 0);

    if (servidor_fd < 0){
        perror("Error al abrir conexión");
    }


    if(servidor_fd > 0){
        int nuevo_cliente_fd = 0;
        int nuevo_cliente_fd_2 = 0;

        while (1){

            // Hasta que establezca apropiadamente una conexión
            while((nuevo_cliente_fd) <= 0){
                nuevo_cliente_fd = aceptar_pedidos(servidor_fd, 0);
                if (nuevo_cliente_fd <= 0){
                    perror("Error al aceptar conexión del cliente 1");
                    nuevo_cliente_fd = NULL_I;
                }
            }

            while(nuevo_cliente_fd_2 <= 0){
                nuevo_cliente_fd_2 = aceptar_pedidos(servidor_fd, 0);
                if (nuevo_cliente_fd_2 <= 0){
                    perror("Error al aceptar conexión del cliente 2");
                    nuevo_cliente_fd_2 = NULL_I;
                }  
            }

            if(nuevo_cliente_fd != NULL_I && nuevo_cliente_fd_2 != NULL_I){
                // Memoria alocadapara el thread: FD's
                int *sumaClientes = (int*)malloc(2*sizeof(int));

                *(sumaClientes) = nuevo_cliente_fd;
                *(sumaClientes+1) = nuevo_cliente_fd_2;
                
                pthread_t hilo;
                pthread_create(&hilo, NULL, serverLoop, sumaClientes);
                pthread_detach(hilo);

                // Reseteando los FD para usarlos como condición en los while
                nuevo_cliente_fd = 0;
                nuevo_cliente_fd_2 = 0;
            }
        }
    }
}