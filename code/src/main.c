//#include <stdio.h>
#include <stdlib.h>
#include "sdl_funcs.h"
#include "BN.h"

int main()
{
    struct Game* game = malloc(sizeof(struct Game));

    initializeWindow(game);
    
    struct BN_Board* board_ = malloc(sizeof(*board_));
    BN_clear_board(board_);

    BN_change_row(board_, 3, 0b00111000);

    while (game->isRunning)
    {
        processInput(game, board_);
        update(game, board_);
        render(game, board_);
    }
    
    freeGame(game);
    return 0;
}
