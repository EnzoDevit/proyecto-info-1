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

char BN_getpos(struct BN_Board*, unsigned char, unsigned char, unsigned char);
void BN_clear_board(struct BN_Board*);
void BN_set_board(struct BN_Board*, Uint64, Uint64);
void BN_change_row(struct BN_Board*, unsigned char, unsigned char, unsigned char);

#endif