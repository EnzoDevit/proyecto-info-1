#include "barco.h"      //COMIT 18/10

Barco* obtenerBarco(int x, int y, Lista* lista){ //lista tiene que apuntar a la cabeza de la lista
    if(lista->cabeza == NULL){
        return NULL;
    }else{
        Barco* puntero = lista->cabeza;
        while(puntero!=NULL){
            if(puntero->x == x || puntero->y == y){
                puntero->Out = 1; //esta funcion se va a implementar cuando el usuario mande coordenadas de ataque, entonces esta funcion las buscara y si estan bien lo tira
                return puntero; //lo tiene q mandar al cliente
            }
            puntero = puntero->siguiente;
        }
        return NULL; //devuelve null porque no le dio a ningun barco
    }
    
}