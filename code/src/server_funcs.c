#include "BN.h"
#include <inttypes.h>
#include "server_funcs.h"

// 1: arriba
// 2: abajo
// 3: izquierda
// 4: derecha

int BN_checkship(struct BN_Board * board, unsigned char xpos, unsigned char ypos, unsigned char dir)
{
    int retval = 1;
    if((ypos < 8)&&(xpos<8))
    {
        if (BN_getpos(board, xpos, ypos, BN_TYPE_SHIP))
        {
            if(!BN_getpos(board, xpos, ypos, BN_TYPE_SHOT)) retval = 0;
            else
            {
                if(dir != 1) retval = BN_checkship(board, xpos, ypos + 1, 2);
                if(retval) if(dir != 2) retval = BN_checkship(board, xpos, ypos - 1, 1);
                if(retval) if(dir != 4) retval = BN_checkship(board, xpos + 1, ypos, 3);
                if(retval) if(dir != 3) retval = BN_checkship(board, xpos - 1, ypos, 4);
            }
        }
    }
    return retval;
}

int BN_checkAllShipsDown(struct BN_Board * board)//te fijas si a cada barco le pegaste un tiro
{
    uint64_t shot = *((uint64_t*) (board->shot));
    uint64_t ship = *((uint64_t*) board->ship);
    return  (ship & (~shot))?0:1;
}

int BN_answerShot(struct BN_Board * board, unsigned char xpos, unsigned char ypos)
{
    BN_setpos(board, xpos, ypos, BN_TYPE_SHOT, 1);
  
    int retval = BN_STATUS_NOHIT;
    if(BN_getpos(board, xpos, ypos, BN_TYPE_SHIP))
    {
        retval = BN_STATUS_HIT;
        if (BN_checkAllShipsDown(board)) retval = BN_STATUS_GAMEWON;
        else if (BN_checkship(board, xpos, ypos,0))  retval = BN_STATUS_SHIPDOWN;
    }

    return retval;
}



void manejar_signal(int signo){
    if (signo == SIGUSR1) {
        //algo
    } else if (signo == SIGUSR2){
        //algo
    }
}