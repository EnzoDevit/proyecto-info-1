#ifndef BN_lib
#define BN_lib

struct BN_Board
{
    char shot[8];
    char boats[8];
};

char BN_getpos(struct BN_Board* b_, unsigned char x, unsigned char y);
void BN_clear_board(struct BN_Board* b_);
void BN_change_row(struct BN_Board* b_, unsigned char y, unsigned char row);

#endif