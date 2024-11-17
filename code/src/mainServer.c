#include "server_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sock-lib.h"
#include <time.h>

#define PORT 8000


int main(){

    srand(time(NULL));
    int servidor_fd = abrir_conexion(PORT, 10, 1);

    if (servidor_fd < 0){
        perror("Error al abrir conexión");
    }


    if(servidor_fd > 0){
    
    int *nuevo_cliente_fd = malloc(sizeof(int));
    int *nuevo_cliente_fd_2 = malloc(sizeof(int));

        while (1){

            while(*nuevo_cliente_fd <= 0){

                *nuevo_cliente_fd = aceptar_pedidos(servidor_fd, 1);

                    if (*nuevo_cliente_fd <= 0){
                        perror("Error al aceptar conexión del cliente 1");
                        *nuevo_cliente_fd = NULL;
                    }
            }

            while(*nuevo_cliente_fd_2 <= 0){
                *nuevo_cliente_fd_2 = aceptar_pedidos(servidor_fd, 1);

                    if (*nuevo_cliente_fd_2 <= 0){
                        perror("Error al aceptar conexión del cliente 2");
                        *nuevo_cliente_fd_2 = NULL;
                    }  
            }

            *nuevo_cliente_fd_2 = aceptar_pedidos(servidor_fd, 1);

                if (*nuevo_cliente_fd_2 <= 0){
                    perror("Error al aceptar conexión del cliente 2");
                    *nuevo_cliente_fd_2 = NULL;
                }

                if(*nuevo_cliente_fd == NULL){
                    perror("Error al aceptar conexión del cliente 1");
                }
                else if(*nuevo_cliente_fd_2 == NULL){
                    perror("Error al aceptar conexión del cliente 2");
                }

                if(*nuevo_cliente_fd != NULL && *nuevo_cliente_fd_2 != NULL){
                    int *sumaClientes = (int)malloc(2*sizeof(int));

                    *(sumaClientes) = *nuevo_cliente_fd;
                    *(sumaClientes+1) = *nuevo_cliente_fd_2;
                    
                    pthread_t hilo;
                    pthread_create(&hilo, NULL, serverLoop, sumaClientes);


                    //pthread_detach(hilo);
                }


        
        }
    }
}