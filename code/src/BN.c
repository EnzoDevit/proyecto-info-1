#include "BN.h"
#include <SDL2/SDL_mouse.h>

char BN_getpos(struct BN_Board* b_, unsigned char x, unsigned char y, unsigned char type)
{
    char retval = -1;
    if ((y < 8)&&(x<8))
    {
        if (type == BN_TYPE_SHOT) retval = (b_->shot[y]&(1<<x))>>x;
        if (type == BN_TYPE_SHIP) retval = (b_->ship[y]&(1<<x))>>x;
    }
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

void BN_set_board(struct BN_Board* b_, Uint64 shot_, Uint64 ship_)
{
    if (b_)
    {
        Uint64* ptr = ((Uint64*)b_);
        *ptr = shot_;
        *(ptr+1) = ship_;
    }
}

void BN_change_row(struct BN_Board* b_, unsigned char y, unsigned char row, unsigned char type)
{
    if (y < 8)
    {
        if (type == BN_TYPE_SHOT) b_->shot[y]=row;
        if (type == BN_TYPE_SHIP) b_->ship[y]=row;
    }
}

//unsigned int BN_get_mouse_pos()
//{
//    
//}