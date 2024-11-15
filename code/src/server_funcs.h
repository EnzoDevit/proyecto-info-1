#include "BN.h"

int BN_checkship(struct BN_Board *, unsigned char x, unsigned char y, unsigned char dir);
int BN_checkAllShipsDown(struct BN_Board *);
int BN_answerShot(struct BN_Board *, unsigned char x, unsigned char y);