#include "sdl_funcs.h"
#include <SDL2/SDL_rect.h>

void handleMouseInput(struct Game* game, SDL_Event event, struct BN_Board* board)
{
    int rx, ry; // rounded x rounded y
    // Se aprovecha el redondeo de división entre enteros
    // para consegui lascoordenadas en casillas
    rx = event.motion.x / (BN_TILE_SIZE + BN_MARGIN_SIZE);
    ry = event.motion.y / (BN_TILE_SIZE + BN_MARGIN_SIZE);

    if (event.button.button== 1)
    {
        if( (rx < BN_COLUMNS)&&
            (ry < BN_COLUMNS)&& // N° de columnas = N° de filas
            // Si el mouse está dentro del cuadrado de la casilla
            ((event.motion.x%(BN_TILE_SIZE + BN_MARGIN_SIZE))>BN_MARGIN_SIZE)&&
            ((event.motion.y%(BN_TILE_SIZE + BN_MARGIN_SIZE))>BN_MARGIN_SIZE))
        {
            if(!BN_getpos(board, rx, ry, BN_TYPE_SHOT))
            {
                // Se escribe el mensaje a enviar
                // El mutex es para que no se acceda al mensaje mientras se edita
                pthread_mutex_lock(&(game->msgmutex));
                game->isTurn = 0;
                game->msg->type = BN_MSGTYPE_ACTION;
                game->msg->x = rx;
                game->msg->y = ry;
                pthread_mutex_unlock(&(game->msgmutex));
            }
        }
    }
    return;
}

int initializeGame(struct Game* game)
{
    int errcode = SDL_Init(SDL_INIT_EVERYTHING);
    game->isRunning = 1;
    game->isTurn = 0;
    game->isWon = 0;
    game->threadEnded = 0;
    game->list = (Node*)0;
    pthread_mutex_init(&(game->msgmutex), NULL);
    if(errcode == 0)
    {
        game->win = SDL_CreateWindow( //Creando el "objeto" (como puntero a struct) de la ventana
            "La gran naval",
            SDL_WINDOWPOS_CENTERED, // La libreria se fija para centrarlo 
            SDL_WINDOWPOS_CENTERED,
            BN_WINDOW_SIZE * 2 + BN_BIG_MARGIN_SIZE,
            BN_WINDOW_SIZE,
            SDL_WINDOW_SHOWN
        );
        if(game->win != 0)
        {// Attached a win, -1 para que elija el driver default, 0 flags
            game->renderer =  SDL_CreateRenderer(game->win, -1, 0); 
            if(game->renderer == 0)
            {
                errcode = DEFAULT_ERROR;
            }
        }
        else
        {
            errcode = DEFAULT_ERROR;
        }
    }
    if (errcode != 0) 
    {
        game->isRunning = 0;
    }
    return errcode;
}

// La funcion que procesa todos los eventos, desde los clicks hasta el boton de cerrar la ventana
void processInput(struct Game* game, struct BN_Board* board, struct BN_Board* board_self)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        // Cerrar la ventana:
        case SDL_QUIT:
            game->isRunning = 0;
            msg_pack msg = {BN_MSGTYPE_GAMEENDED, 0, 0};
            write(game->sd, &msg, sizeof(msg));
            break;
        
        // Al hacer click
        case SDL_MOUSEBUTTONDOWN:
            if(game->isTurn)
                handleMouseInput(game, event, board);
            break;

        default:
            break;
    }
}

// Funcion de renderizado llamada cada tick
void render(struct Game* game, struct BN_Board* board, struct BN_Board* board_self)
{
    //Rellenando la ventana de negro cada frame para limpiar lo ultimo
    SDL_SetRenderDrawColor(game->renderer, BN_BLACK); // Negro
    SDL_RenderClear(game->renderer);
    
    //El espaciador entre los tableros:
    SDL_Rect spacer ={BN_WINDOW_SIZE, 0, BN_BIG_MARGIN_SIZE, BN_WINDOW_SIZE};

    SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
    SDL_RenderFillRect(game->renderer, &spacer);    

    SDL_Rect rect ={0, 0, BN_TILE_SIZE, BN_TILE_SIZE};
    SDL_Rect dot ={0, 0, BN_DOT_SIZE, BN_DOT_SIZE};
    SDL_Rect ship ={0, 0, BN_SHIP_SIZE, BN_SHIP_SIZE};
    

    // Va tile por tile dibujando el contorno y, condicionalmente los barcos y disparos
    for (int y = 0; y < BN_COLUMNS; y++)
    {
        rect.y = BN_MARGIN_SIZE +  y*(BN_MARGIN_SIZE + BN_TILE_SIZE);
        ship.y = rect.y + BN_SHIP_MARGIN_SIZE;
        dot.y = rect.y + BN_DOT_MARGIN_SIZE;

        // Renderizado del tablero deloponente

        for (int x = 0; x < BN_COLUMNS; x++)
        {
            // Contorno
            rect.x = BN_MARGIN_SIZE +  x*(BN_MARGIN_SIZE + BN_TILE_SIZE);
            SDL_RenderDrawRect(game->renderer, &rect);
            
            // Barco
            if(BN_getpos(board, x, y, BN_TYPE_SHIP) == 1)
            {
                ship.x = rect.x + BN_SHIP_MARGIN_SIZE;
                SDL_SetRenderDrawColor(game->renderer, BN_WHITE);
                SDL_RenderFillRect(game->renderer, &ship);
                SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
            }
            
            // Disparo
            if(BN_getpos(board, x, y, BN_TYPE_SHOT) == 1)
            {
                dot.x = rect.x + BN_DOT_MARGIN_SIZE;
                SDL_SetRenderDrawColor(game->renderer, BN_RED);
                SDL_RenderFillRect(game->renderer, &dot);
                SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
            }
        }

        // Ahora lo mismo pero con el tablero propio
        
        for (int x = 0; x < BN_COLUMNS; x++)
        {
            rect.x = BN_WINDOW_SIZE + BN_BIG_MARGIN_SIZE + BN_MARGIN_SIZE +  x*(BN_MARGIN_SIZE + BN_TILE_SIZE);

            SDL_RenderDrawRect(game->renderer, &rect);
            if(BN_getpos(board_self, x, y, BN_TYPE_SHIP) == 1)
            {
                ship.x = rect.x + BN_SHIP_MARGIN_SIZE;
                SDL_SetRenderDrawColor(game->renderer, BN_BLUE);
                SDL_RenderFillRect(game->renderer, &ship);
                SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
            }
            if(BN_getpos(board_self, x, y, BN_TYPE_SHOT) == 1)
            {
                dot.x = rect.x + BN_DOT_MARGIN_SIZE;
                SDL_SetRenderDrawColor(game->renderer, BN_YLW);
                SDL_RenderFillRect(game->renderer, &dot);
                SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
            }
        }
        
    }

    // Se renderizan los barcos caidos sacados de la lista
    Node* p = game->list;
    SDL_SetRenderDrawColor(game->renderer, BN_FALLEN);
    while (p) {
        SDL_RenderFillRect(game->renderer, &(p->rect));
        p = p->next;
    }

    //Se dibuja el recuadro que señalizaquien tiene el turno
    SDL_Rect frame = {BN_FRAME_OFFSET_SIZE, BN_FRAME_OFFSET_SIZE, BN_WINDOW_SIZE - 2*BN_FRAME_OFFSET_SIZE,BN_WINDOW_SIZE - 2*BN_FRAME_OFFSET_SIZE};
    if(game->isTurn)
    {
        SDL_SetRenderDrawColor(game->renderer, BN_GREEN);
    }
    else {
        SDL_SetRenderDrawColor(game->renderer, BN_RED);
        frame.x += BN_WINDOW_SIZE + BN_BIG_MARGIN_SIZE;
    }
    SDL_RenderDrawRect(game->renderer, &frame);
    
    // Se presenta todo lo dibujado al buffer de la pantalla
    SDL_RenderPresent(game->renderer);
}


// Se encargade liberar primero las cosas del SDL y luego el espacio de game
void freeGame(struct Game* game)
{
    if(game->renderer)
        SDL_DestroyRenderer(game->renderer);
    if(game->win)
        SDL_DestroyWindow(game->win);
    SDL_Quit(); // Cierra SDL

    free(game);
}


void endGame(struct Game* game)
{
    if(game->isWon)
        printf("Ganaste el Juego!\n");
    else
        printf("Perdiste el juego...\n"); 
}