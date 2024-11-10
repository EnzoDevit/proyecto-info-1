//#include <stdio.h>
#include <stdlib.h>
#include "sdl_funcs.h"
#include "BN.h"

int main()
{
    struct Game* game = malloc(sizeof(struct Game));

    initializeWindow(game);
    
    struct BN_Board* board_ = malloc(sizeof(*board_));
    struct BN_Board* board_self = malloc(sizeof(*board_self));
    
    //BN_clear_board(board_);

    //BN_change_row(board_, 3, 0b00111000, BN_TYPE_SHIP);

    BN_set_board(board_, 0x030C00, 0x020A00);
    BN_set_board(board_self, 0x030C00, 0x020A00);

    while (game->isRunning)
    {
        processInput(game, board_, board_self);
        update(game, board_, board_self);
        render(game, board_, board_self);
    }
    
    freeGame(game);
    return 0;
}
