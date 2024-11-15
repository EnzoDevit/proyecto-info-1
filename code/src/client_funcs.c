#include "client_funcs.h"


void BN_processResponse(struct Game* game, struct BN_Board* board, unsigned char x, unsigned char y, int response)
{
      
    if(response != BN_STATUS_NOHIT)
    {
        BN_setpos(board, x, y, BN_TYPE_SHOT, 1);
        // TODO al tirar un barco if (response == BN_STATUS_SHIPDOWN) {}
        if (response == BN_STATUS_GAMEWON)
        {
            game->isRunning = 0;
            game->isWon = 1;
        }
    }
}

