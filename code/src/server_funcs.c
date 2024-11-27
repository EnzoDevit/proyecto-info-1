#include "server_funcs.h"

// 1: arriba
// 2: abajo
// 3: izquierda
// 4: derecha



// Funcion que recorre recursivamente el barco para determinar si está caido
int BN_checkship(struct BN_Board * board, unsigned char xpos, unsigned char ypos, unsigned char dir)
{
    int retval = 1;
    if((ypos < 8)&&(xpos<8))
    {
        if (BN_getpos(board, xpos, ypos, BN_TYPE_SHIP))
        {

            if(!BN_getpos(board, xpos, ypos, BN_TYPE_SHOT)) retval = 0;
            else if (dir == 0)
            {
                retval = BN_checkship(board, xpos, ypos + 1, 2);
                if(retval) retval = BN_checkship(board, xpos, ypos - 1, 1);
                if(retval) retval = BN_checkship(board, xpos + 1, ypos, 3);
                if(retval) retval = BN_checkship(board, xpos - 1, ypos, 4);
            
            }
            else
            {
                // Sigue para el mismo lado
                if(dir == 1) retval = BN_checkship(board, xpos, ypos - 1, 1);
                else if(dir == 2) retval = BN_checkship(board, xpos, ypos + 1, 2);
                else if(dir == 3) retval = BN_checkship(board, xpos + 1, ypos, 3);
                else if(dir == 4) retval = BN_checkship(board, xpos - 1, ypos, 4);
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

// Toma el mensaje de disparo y lo ejeduta en el server 
int BN_answerShot(struct BN_Board * board, unsigned char xpos, unsigned char ypos)
{
    BN_setpos(board, xpos, ypos, BN_TYPE_SHOT, 1);
  
    int retval = BN_STATUS_NOHIT;
    if(BN_getpos(board, xpos, ypos, BN_TYPE_SHIP))
    {
        retval = BN_STATUS_HIT;
        if (BN_checkAllShipsDown(board)) retval = BN_STATUS_GAMEWON;
        else if (BN_checkship(board, xpos, ypos,0))  retval = BN_STATUS_SHIPDOWN;
    }

    return retval;
}



void manejar_signal(int signo){
    if (signo == SIGUSR1) {
        //algo
    } else if (signo == SIGUSR2){
        //algo
    }
}



void* serverLoop(void* data)
{
    struct sigaction new_actn, old_actn;
    new_actn.sa_handler = SIG_IGN;

    // Para que un error repentino de conexión no tire abajo el servidor
    // Si el write o read devuelven -1 no se hace kill al programa
    sigemptyset (&new_actn.sa_mask);
    new_actn.sa_flags = 0;
    sigaction (SIGPIPE, &new_actn, &old_actn);

    BN_Board* boards = malloc(2*sizeof(BN_Board));
    uint64_t states[] = BN_PRESAVED_STATES;
    int random1, random2;
    random1 = rand()%9;
    BN_set_board(boards, 0, states[random1]);
    // Busca un numero diferente
    do {random2 = rand()%9;}while(random1 == random2);
    BN_set_board(boards+1, 0, states[random2]);

    // consiguiendo los FD de lo pasado desde el main
    int* sd = (int*) data;
    write(*(sd), (void*)(boards), sizeof(BN_Board));
    write(*(sd+1), (void*)(boards+1), sizeof(BN_Board));
    
    int aux = 0;
    char running = 1;
    char turn = 0;

    msg_pack* msg = malloc(sizeof(msg_pack));
    // mensaje a enviar
    msg_pack* msg_s;
    // Se anuncia el inicio del juego con este mensaje
    msg_pack msg_obj = {BN_MSGTYPE_GAMESTARTED, 0, 0};

    *(unsigned char*)msg = 0;
    msg_s = &msg_obj;

    msg_s->type = BN_MSGTYPE_GAMESTARTED;

    // Le escribe al primer cliente 
    write(*(sd), (void*)(msg_s), sizeof(msg_pack));

    while (running) {
        // Lee el disparo
        read(*(sd + turn),  (void*)(msg), sizeof(msg_pack));
        
        if(msg->type == BN_MSGTYPE_ACTION)
        {
            *(unsigned char*)msg_s = 0;
            // establecemos la convencion de mandar las respuesteas en el x
            msg_s->type = BN_MSGTYPE_ACTION;
            msg_s->x = BN_answerShot(boards + ((turn + 1)%2),msg->x,msg->y);
            
            if( msg_s->x== BN_STATUS_GAMEWON) 
            {
                msg_s->type = BN_MSGTYPE_GAMEENDED;
                msg->type = BN_MSGTYPE_GAMEENDED;
            }
            
            // Notifica de la respuesta al disparo al atacante
            write(*(sd + turn),           (void*)(msg_s), sizeof(msg_pack));
            // Envia las coordenadas del disparo al atacado
            write(*(sd + ((turn + 1)%2)), (void*)(msg)  , sizeof(msg_pack));

            // Si falla el tiro, cambia el turno
            if((msg_s->x) == BN_STATUS_NOHIT)
            {
                // turn cambia entre 1 y 0;
                turn ^= 1;
            }
        }
        else running = 0;
    }

    close(*sd);
    close(*(sd +1));


    free(data); //libera sumaclientes de mainServer.c
    free(msg); 
    free(boards);
    return NULL;
}