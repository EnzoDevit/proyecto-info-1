#include "client_funcs.h"

//MALLOCEO DE LA VARIABLE msg Y DESPUES SE LE HACE FREE DESPUES DEL WHILE
// Cliente ejecuta  el mensage que le devuelve el serveral disparar

/* 
 * Funcion que maneja la respuesta del servidor en el cliente
 */
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

/* Una funcion para recorrer a lo largo de un barco
 * si la vatiable is_x es !=0 entonces recorre en x, si no recorre en y
 * Dependiendo de la direccion 1 o -1 devuelve la minimo o maxima posicion del barco
 */
static int shipWalk(BN_Board* board,  int x,  int y, char is_x, char dir)
{
    int* changed = is_x? &x:&y; // condiciono la que va a ser cambiada
    char run_loop = 1;
    while (run_loop){
        *changed += dir;
        if (((*changed)<0)||((*changed>7)))run_loop = 0;
        else if(!BN_getpos(board, x, y, BN_TYPE_SHIP))run_loop = 0;
    }
    *changed -= dir;
    return *changed;
}


// Agrega a la lista de barcos el barco que pasa por la casilla (x;y)
void BN_getShip(BN_Board* board, Node** list ,unsigned int x, unsigned int y)
{
    char size_x, size_y, min_x, min_y;
    min_x = shipWalk(board, x,y,1,-1);
    size_x = shipWalk(board, x,y,1,1) - min_x + 1;
    min_y = shipWalk(board, x,y,0,-1);
    size_y = shipWalk(board, x,y,0,1) - min_y + 1;
    SDL_Rect ship = {
        BN_SHIP_DOWN_OFFSET_SIZE + min_x*(BN_MARGIN_SIZE + BN_TILE_SIZE),
        BN_SHIP_DOWN_OFFSET_SIZE + min_y*(BN_MARGIN_SIZE + BN_TILE_SIZE),
        size_x*(BN_TILE_SIZE + BN_MARGIN_SIZE) - 2*BN_SHIP_DOWN_OFFSET_SIZE + BN_MARGIN_SIZE, 
        size_y*(BN_TILE_SIZE + BN_MARGIN_SIZE) - 2*BN_SHIP_DOWN_OFFSET_SIZE + BN_MARGIN_SIZE
    };
    Node* newShip = (Node*)malloc(sizeof(Node));
    newShip->rect = ship;
    newShip->next = *list;
    *list = newShip;
}
// */

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
    /* */
    if((msg->type) == BN_MSGTYPE_ACTION) 
    {
        BN_setpos(boards, msg->x, msg->y, BN_TYPE_SHOT, 1);
    
        if (!BN_getpos(boards, msg->x, msg->y, BN_TYPE_SHIP))
        {
            game->isTurn = 1;
        }
    }
    else 
    {
        game->isTurn = 1;
    }
    while (game->isRunning) {

        if((game->isTurn) == 1)
        {
            // isTurn se pone en 0 cuando se toca una casilla durante el turno
            while (game->isTurn)
            {
                if((game->isRunning)==0) 
                {
                    break;
                }
            }
            
            // bloquea el mutex para escribir el mensage que 
            // es modificado en la parte de SDL
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
                if((msg->x) == BN_STATUS_SHIPDOWN)
                {
                    BN_getShip(boards + 1, &(game->list), msg_s->x, msg_s->y);
                }
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
    // */
    game->isTurn = 0;
    game->threadEnded = 1;
    
    return 0;
}