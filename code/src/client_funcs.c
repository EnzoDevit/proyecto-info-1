#include "client_funcs.h"

//MALLOCEO DE LA VARIABLE msg Y DESPUES SE LE HACE FREE DESPUES DEL WHILE
// Cliente ejecuta  el mensaje que le devuelve el serveral disparar

/* 
 * Funcion que maneja la respuesta del servidor en el cliente
 * Funcion meramente de logica
 */
void BN_processResponse(Game* game, BN_Board* board, unsigned char x, unsigned char y, unsigned char statustype)
{
    BN_setpos(board, x, y, BN_TYPE_SHOT, 1);
    if(statustype != BN_STATUS_NOHIT)
    {
        BN_setpos(board, x, y, BN_TYPE_SHIP, 1);
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
    // Camina el barco hasta la izquierda para encontrar el menor
    min_x = shipWalk(board, x,y,1,-1);
    // Camina hasta la derecha hasta encontrar el maximo, se resta
    // y como se le resta el absoluto, se le sumala casilla 0
    size_x = shipWalk(board, x,y,1,1) - min_x + 1;
    min_y = shipWalk(board, x,y,0,-1);
    size_y = shipWalk(board, x,y,0,1) - min_y + 1;

    // El rectangulo que representa el barco
    SDL_Rect ship = {
        BN_SHIP_DOWN_OFFSET_SIZE + min_x*(BN_MARGIN_SIZE + BN_TILE_SIZE), // x
        BN_SHIP_DOWN_OFFSET_SIZE + min_y*(BN_MARGIN_SIZE + BN_TILE_SIZE), // y
        size_x*(BN_TILE_SIZE + BN_MARGIN_SIZE) - 2*BN_SHIP_DOWN_OFFSET_SIZE + BN_MARGIN_SIZE, // w
        size_y*(BN_TILE_SIZE + BN_MARGIN_SIZE) - 2*BN_SHIP_DOWN_OFFSET_SIZE + BN_MARGIN_SIZE  // h
    };
    Node* newShip = (Node*)malloc(sizeof(Node));

    // inserto el barco caido
    newShip->rect = ship;
    newShip->next = *list;
    *list = newShip;
}


// Hay que reservar previamente los componentes de data
// Es el loop que se llama desde mainCliente como thread
void* clientLoop(void* data)
{
    //inicialización de las variables locales

    int sd = ((client_data*)data)->sock_descriptor;
    Game* game = ((client_data*)data)->game;

    BN_Board* board = ((client_data*)data)->board1;
    BN_Board* board_opp = ((client_data*)data)->board2;

    // Limpia los tableros
    BN_clear_board(board);
    BN_clear_board(board_opp);

    // mensaje a recibir
    msg_pack msg__ = {0,0,0};
    // mansage a enviar
    msg_pack* msg_s = game->msg;
    
    char running = 1;

    // Recibe un mensaje que tiene como contenido el tablero propio
    read(sd, (void*)(board), sizeof(BN_Board));

    // Recibe un mensaje
    read((sd), (void*)( &msg__), sizeof(msg_pack));
    // Solo si es un tiro lo procesa
    if(( msg__.type) == BN_MSGTYPE_ACTION) 
    {
        BN_setpos(board,  msg__.x,  msg__.y, BN_TYPE_SHOT, 1);
    
        // si el oponente no le acierta
        if (!BN_getpos(board,  msg__.x,  msg__.y, BN_TYPE_SHIP))
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
            while ((game->isTurn)&&(game->isRunning));
            
            // bloquea el mutex para escribir el mensaje que 
            // es modificado en la parte de SDL
            pthread_mutex_lock(&(game->msgmutex));
            // Se envia el tiro
            write(sd,  (void*)(msg_s), sizeof(msg_pack));
            pthread_mutex_unlock(&(game->msgmutex));

            // Respuesta al tiro del server
            read(sd, (void*)( &msg__), sizeof(msg_pack));
            
            BN_processResponse(game, board_opp, msg_s->x, msg_s->y,  msg__.x);
            
            if( msg__.type == BN_MSGTYPE_GAMEENDED)
            {
                // Termina el juego
                game->isTurn = 1;
                game->isRunning = 0;
                game->isWon = 1;
            }
            // Si el jugador acertó
            if(( msg__.x) != BN_STATUS_NOHIT)
            {
                game->isTurn = 1;
                if(( msg__.x) == BN_STATUS_SHIPDOWN)
                {
                    // Agrega el barco caído
                    BN_getShip(board_opp, &(game->list), msg_s->x, msg_s->y);
                }
            }
        }
        if((game->isTurn) == 0)
        {
            // Tiro del oponente
            read(sd, (void*)( &msg__)  , sizeof(msg_pack));
            BN_setpos(board,  msg__.x,  msg__.y, BN_TYPE_SHOT, 1);
            if(( msg__.type)== BN_MSGTYPE_GAMEENDED){
                game->isRunning = 0;
            } // si el oponente no acertó
            else if (!BN_getpos(board,  msg__.x,  msg__.y, BN_TYPE_SHIP)) {
                game->isTurn = 1;
            }
        }
    } 

    game->isTurn = 0;
    game->threadEnded = 1;
    
    return 0;
}