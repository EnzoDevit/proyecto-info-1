#include "client_funcs.h"
//#include <stdio.h>
#include <unistd.h>

//MALLOCEO DE LA VARIABLE msg Y DESPUES SE LE HACE FREE DESPUES DEL WHILE
// Cliente ejecuta  el mensage que le devuelve el serveral disparar

void BN_processResponse(Game* game, BN_Board* board, unsigned char x, unsigned char y, unsigned char statustype)
{
    BN_setpos(board, x, y, BN_TYPE_SHOT, 1);
    if(statustype != BN_STATUS_NOHIT)
    {
        BN_setpos(board, x, y, BN_TYPE_SHIP, 1);
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
    //printf("comienza"); fflush(stdout);
    int sd = ((client_data*)data)->sock_descriptor;
    Game* game = ((client_data*)data)->game;

    BN_Board* boards = ((client_data*)data)->board1;

    BN_clear_board(boards + 1);

    msg_pack msg__ = {0,0,0};

    msg_pack* msg = &msg__;// mensage a recibir //malloc casteado
    
    msg_pack* msg_s = game->msg;
    
    char running = 1;

    read(sd, (void*)(boards), sizeof(BN_Board));
//    printf("llega"); fflush(stdout);
    read((sd), (void*)(msg), sizeof(msg_pack));
//    printf("tipo: %d, x: %d, y: %d\n", msg->type, msg->x, msg->y);fflush(stdout);
    
    if((msg->type) == BN_MSGTYPE_ACTION) 
    {
        BN_setpos(boards, msg->x, msg->y, BN_TYPE_SHOT, 1);
    
        if (!BN_getpos(boards, msg->x, msg->y, BN_TYPE_SHIP))
        {
            game->isTurn = 1;
            //printf("Pifia"); fflush(stdout);
        }
        else 
        {
            //printf("Acierta"); fflush(stdout);
        }
    }
    else 
    {
        game->isTurn = 1;
        //printf("Inicio"); fflush(stdout);
    }
    while (game->isRunning) {

        if((game->isTurn) == 1)
        {
            while (game->isTurn)
            {
                if((game->isRunning)==0) 
                {
                    //msg_s->type = BN_MSGTYPE_GAMEENDED;
                    //pthread_mutex_lock(&(game->msgmutex));
                    //write(sd,  (void*)(msg_s), sizeof(msg_pack));
                    //pthread_mutex_unlock(&(game->msgmutex));
                    break;
                }
            }
            
            pthread_mutex_lock(&(game->msgmutex));
            write(sd,  (void*)(msg_s), sizeof(msg_pack));
            pthread_mutex_unlock(&(game->msgmutex));

            read(sd, (void*)(msg), sizeof(msg_pack));
            
            BN_processResponse(game, boards+1, msg_s->x, msg_s->y, msg->x);
            
            if(msg->type == BN_MSGTYPE_GAMEENDED)
            {
                game->isTurn = 1;
                game->isRunning = 0;
                game->isWon = 1;
            }
            if((msg->x) != BN_STATUS_NOHIT)
            {
                game->isTurn = 1;
            }
        }
        if((game->isTurn) == 0)
        {
            read(sd, (void*)(msg)  , sizeof(msg_pack));
            BN_setpos(boards, msg->x, msg->y, BN_TYPE_SHOT, 1);
            if((msg->type)== BN_MSGTYPE_GAMEENDED){
                game->isRunning = 0;
            }
            else if (!BN_getpos(boards, msg->x, msg->y, BN_TYPE_SHIP)) {
                game->isTurn = 1;
            }
        }
    }
    
    game->isTurn = 0;
    game->threadEnded = 1;

    return 0;
}