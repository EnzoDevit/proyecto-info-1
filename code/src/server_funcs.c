#include "BN.h"

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