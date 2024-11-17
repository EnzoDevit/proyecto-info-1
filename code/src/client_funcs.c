#include "client_funcs.h"

#include <unistd.h>


// Cliente ejecuta  el mensage que le devuelve el serveral disparar
void BN_processResponse(Game* game, BN_Board* board, unsigned char x, unsigned char y, unsigned char statustype)
{
      
    if(statustype != BN_STATUS_NOHIT)
    {
        BN_setpos(board, x, y, BN_TYPE_SHOT, 1);
        // TODO al tirar un barco if (response == BN_STATUS_SHIPDOWN) {}
        if (statustype == BN_STATUS_GAMEWON)
        {
            game->isRunning = 0;
            game->isWon = 1;
        }
    }
}


// Hay que reservar previamente los componentes de data
void* clientLoop(void* data)
{
    int sd = ((client_data*)data)->sock_descriptor;
    Game* game = ((client_data*)data)->game;

    BN_Board* boards = ((client_data*)data)->board1;

    BN_clear_board(boards + 1);

    msg_pack* msg = malloc(sizeof(msg_pack));// mensage a recibir
    *(unsigned char*)msg = 0;//pone las 3 variables en 0
    
    msg_pack* msg_s = game->msg;
    
    char running = 1;

    read(sd, (void*)(boards), sizeof(BN_Board));

    read((sd), (void*)(&msg), sizeof(msg_pack));

    if(msg->type == BN_MSGTYPE_ACTION) 
    {
        BN_setpos(boards, msg->x, msg->y, BN_TYPE_SHOT, 1);
    }

    while (running) {

        game->isTurn = 1;

        while (game->isTurn);
        
        pthread_mutex_lock(&(game->msgmutex));
        write(sd,  (void*)(msg_s), sizeof(msg_pack));
        pthread_mutex_unlock(&(game->msgmutex));

        read(sd, (void*)(msg), sizeof(msg_pack));
        
        BN_processResponse(game, boards+1, msg_s->x, msg_s->y, msg->x);
        
        if(msg->type == BN_MSGTYPE_GAMEENDED)
        {
            running = 0;
            game->isWon = 1;
        }
        else 
        {
            read(sd, (void*)(msg)  , sizeof(msg_pack));
            BN_setpos(boards, msg->x, msg->y, BN_TYPE_SHOT, 1);
            if(msg->type == BN_MSGTYPE_GAMEENDED){
                running = 0;
            }
        }
    }
    return 0;
}