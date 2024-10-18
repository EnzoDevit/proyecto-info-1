#ifndef BARCO_H
#define BARCO_H

#include <stdio.h>
#include <stdlib.h>

#define TAM_TABLERO 8 // Tamaño del tablero

typedef struct Barco {
    int id;          // Identificador del barco
    int x;          // Posición X
    int y;          // Posición Y
    unsigned char Out; //si es 0 el barco esta vivo, si es 1 esta Out
    struct Barco* siguiente; // Puntero al siguiente barco
} Barco;

typedef struct Lista{
    Barco* cabeza;
}Lista;

// Funciones
Barco* agregar_barco(Barco* cabeza, int id, int x, int y);
Barco* obtenerBarco(int x, int y, Lista* lista); //COMIT 18/10
void atacarBarco(Barco* cabeza);
void eliminar_barco(Barco* cabeza);  //COMIT 18/10
void barcoOut(Barco* cabeza);
void mostrar_barcos(Barco* cabeza);
void liberar_barcos(Barco* cabeza);

#endif // BARCO_H
