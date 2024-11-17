#include <SDL2/SDL_stdinc.h>

#ifndef BN_lib
#define BN_lib

#define BN_TYPE_SHOT 0
#define BN_TYPE_SHIP 1


#define BN_STATUS_NOHIT 0
#define BN_STATUS_HIT 1
#define BN_STATUS_SHIPDOWN 2
#define BN_STATUS_GAMEWON 3

#define BN_PRESAVED_STATES {0x36004f404003e00, 0x4052524240001ec0, 0x1e007c000d6101, 0x8c808181a1202620, 0x3e00809e802026, 0x9f800006e0001e00, 0x3c000d41494901, 0x2020888a8282801e, 0x71010d017c001800}

typedef struct BN_Board
{
    char shot[8];
    char ship[8];
}BN_Board;

#define BN_MSGTYPE_ACTION 0
#define BN_MSGTYPE_GAMESTARTED 1
#define BN_MSGTYPE_GAMEENDED 2

typedef struct msg_pack
{
    unsigned char type:2;
    unsigned char x:3;
    unsigned char y:3;
}msg_pack;

char BN_getpos(struct BN_Board*,  unsigned char x, unsigned char y, unsigned char type);
char BN_setpos(struct BN_Board*, unsigned char x, unsigned char y, unsigned char type, unsigned char _bit);
void BN_clear_board(struct BN_Board* );
void BN_set_board(struct BN_Board*, Uint64 shot_, Uint64 ship_);
void BN_change_row(struct BN_Board*, unsigned char y, unsigned char row, unsigned char type);

void BN_print_board(struct BN_Board*);

#endif