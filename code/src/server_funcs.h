#include "BN.h"
#include <stdio.h>//agregada
#include <signal.h>//agregada
#include <pthread.h>//agregada
#include "sock-lib.h"//agregada
#include <stdlib.h>//agregada
#include <string.h>//agregada
#include <unistd.h>//agregada
#include <time.h>

#define NULL_I (0)


int BN_checkship(struct BN_Board *, unsigned char x, unsigned char y, unsigned char dir);
int BN_checkAllShipsDown(struct BN_Board *);
int BN_answerShot(struct BN_Board *, unsigned char x, unsigned char y);
void manejar_signal(int signo);
void* serverLoop(void* data);
