#include "BN.h"
#include <stdint.h>
#include <stdlib.h>

#include <unistd.h>
#include <SDL2/SDL_stdinc.h>

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
        if (BN_checkAllShipsDown(board) ) retval = BN_STATUS_GAMEWON;
        else if (BN_checkship(board, xpos, ypos,0))  retval = BN_STATUS_SHIPDOWN;
    }

    return retval;
}



void* serverLoop(void* data)
{
    BN_Board* boards = malloc(2*sizeof(BN_Board));
    uint64_t states[] = BN_PRESAVED_STATES;
    BN_set_board(boards, 0, rand()%7) ;
    BN_set_board(boards+1, 0, rand()%7) ;

    int* sd = (int*) data;

    write(*(sd), (void*)(boards), sizeof(BN_Board));
    write(*(sd+1), (void*)(boards+1), sizeof(BN_Board));
    
    int aux = 0;
    char running = 1;
    char turn = 0;

    msg_pack* msg = malloc(sizeof(msg_pack));
    // mensage a enviar
    msg_pack* msg_s = malloc(sizeof(msg_pack));

    *(unsigned char*)msg = 0;
    *(unsigned char*)msg_s = 0;



    write(*(sd), (void*)(&msg_s), sizeof(msg_pack));

    while (running) {

        read(*(sd + turn),  (void*)(msg), sizeof(msg_pack));

        if(msg->type != BN_MSGTYPE_ACTION) break;
        
        *(unsigned char*)msg_s = 0;
        // establecemos la convencion de mandar las respuesteas en el x
        msg_s->x = BN_answerShot(boards + ((turn + 1)%2),msg->x,msg->y);
        
        if( msg_s->x== BN_STATUS_GAMEWON) 
        {
            msg_s->type = BN_MSGTYPE_GAMEENDED;
            msg->type = BN_MSGTYPE_GAMEENDED;
        }
        
        write(*(sd + turn),           (void*)(msg_s), sizeof(msg_pack));
        write(*(sd + ((turn + 1)%2)), (void*)(msg)  , sizeof(msg_pack));


        turn = (turn + 1)%2;
    }
}