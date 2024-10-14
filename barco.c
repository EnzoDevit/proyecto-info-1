#include "barco.h"

// Agregar un barco a la lista
Barco* agregar_barco(Barco* cabeza, int id, int x, int y) {
    // Verificar si la posición está ocupada
    Barco* actual = cabeza;
    while (actual != NULL) {
        if (actual->x == x && actual->y == y) {
            printf("Ya hay un barco en esta posición (%d, %d).\n", x, y);
            return cabeza; // Fallo, posición ocupada
        }
        actual = actual->siguiente;
    }

    // Crear un nuevo barco
    Barco* nuevo_barco = (Barco*)malloc(sizeof(Barco));
    nuevo_barco->id = id;
    nuevo_barco->x = x;
    nuevo_barco->y = y;
    nuevo_barco->siguiente = cabeza; // Agregar al inicio de la lista

    printf("Barco %d agregado en (%d, %d).\n", id, x, y);
    return nuevo_barco; // Nueva cabeza de la lista
}

// Mostrar todos los barcos en la lista
void mostrar_barcos(Barco* cabeza) {
    Barco* actual = cabeza;
    printf("Barcos en el juego:\n");
    while (actual != NULL) {
        printf("Barco %d: (%d, %d)\n", actual->id, actual->x, actual->y);
        actual = actual->siguiente;
    }
}

// Liberar la memoria de la lista de barcos
void liberar_barcos(Barco* cabeza) {
    Barco* actual = cabeza;
    while (actual != NULL) {
        Barco* siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}
