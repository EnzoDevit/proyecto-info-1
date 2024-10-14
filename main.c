#include "barco.h"

int main() {
    Barco* cabeza_barcos = NULL; // Puntero a la cabeza de la lista de barcos

    // Ejemplo de ingreso de barcos
    cabeza_barcos = agregar_barco(cabeza_barcos, 1, 2, 3); // Barco 1 en (2, 3)
    cabeza_barcos = agregar_barco(cabeza_barcos, 2, 4, 5); // Barco 2 en (4, 5)
    cabeza_barcos = agregar_barco(cabeza_barcos, 3, 2, 3); // Intento de agregar barco en (2, 3) - ya ocupado

    // Mostrar barcos
    mostrar_barcos(cabeza_barcos);

    // Liberar la memoria de los barcos
    liberar_barcos(cabeza_barcos);

    return 0;
}
