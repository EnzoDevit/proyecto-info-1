#include <SDL2/SDL_stdinc.h>

#ifndef BN_lib
#define BN_lib

#define BN_TYPE_SHOT 0
#define BN_TYPE_SHIP 1

struct BN_Board
{
    char shot[8];
    char ship[8];
};

char BN_getpos(struct BN_Board*,  unsigned char x, unsigned char y, unsigned char type);
char BN_setpos(struct BN_Board*, unsigned char x, unsigned char y, unsigned char type, unsigned char _bit);
void BN_clear_board(struct BN_Board*);
void BN_set_board(struct BN_Board*, Uint64 shot_, Uint64 ship_);
void BN_change_row(struct BN_Board*, unsigned char y, unsigned char row, unsigned char type);

#endif