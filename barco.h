#ifndef BARCO_H
#define BARCO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Barco {
    int id;          // Identificador del barco
    int x;          // Posición X
    int y;          // Posición Y
    struct Barco* siguiente; // Puntero al siguiente barco
} Barco;

// Funciones
Barco* agregar_barco(Barco* cabeza, int id, int x, int y);
void mostrar_barcos(Barco* cabeza);
void liberar_barcos(Barco* cabeza);

#endif // BARCO_H
