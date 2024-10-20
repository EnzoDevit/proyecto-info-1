#include "BN.h"
#include <SDL2/SDL_stdinc.h>

char BN_getpos(struct BN_Board* b_, unsigned char x, unsigned char y)
{
    char retval = -1;
    if ((y < 7)&&(x<7))
        retval = (b_->shot[y]&(1<<x))>>x;
    return retval;
}

void BN_clear_board(struct BN_Board* b_)
{
    if (b_)
    {
        Uint64* ptr = ((Uint64*)b_);
        *ptr = 0;
        *(ptr+1) = 0;
    }
}

void BN_change_row(struct BN_Board* b_, unsigned char y, unsigned char row)
{
    if (y < 7)
        b_->shot[y]=row;
}