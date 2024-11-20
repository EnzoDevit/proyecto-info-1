//#include <stdio.h>
#include <stdlib.h>
#include "sdl_funcs.h"
#include "BN.h"

//VARIABLES: game, board_, board_self. CASTEADAS DESPUES DEL MALLOC Y FREE APLICADO DESPUES DE QUE SE TERMINA EL JUEGO(DESPUES DEL WHILE)

int main()
{
    struct Game* game = (Game*)malloc(sizeof(struct Game)); //casteo agregado

    initializeWindow(game);
    

    
    struct BN_Board* board_ = (BN_Board*)malloc(sizeof(*board_)); //casteo agregado
    struct BN_Board* board_self = (BN_Board*)malloc(sizeof(*board_self)); //casteo agregado
    
    //BN_clear_board(board_);

    //BN_change_row(board_, 3, 0b00111000, BN_TYPE_SHIP);

    BN_set_board(board_, 0, 0x36004f404003e00);
    BN_set_board(board_self, 0x40524241ec0, 0x1e007c000d6101);

    while (game->isRunning)
    {
        processInput(game, board_, board_self);
        update(game, board_, board_self);
        render(game, board_, board_self);
    }


    free(game);
    free(board_);
    free(board_self);
    endGame(game);
    freeGame(game);

    return 0;
}
