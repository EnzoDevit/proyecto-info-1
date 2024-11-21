//#include "server_funcs.h"
#include "client_funcs.h"
#include "sdl_funcs.h"
#include "BN.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <pthread.h>
#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>


void handleMouseInput(struct Game* game, SDL_Event event, struct BN_Board* board)
{
    int rx, ry; // rounded x rounded y
    rx = event.motion.x / (BN_TILE_SIZE + BN_MARGIN_SIZE);
    ry = event.motion.y / (BN_TILE_SIZE + BN_MARGIN_SIZE);

    if(
        (rx < BN_COLUMNS)&&
        (ry < BN_COLUMNS)&&
        ((event.motion.x%(BN_TILE_SIZE + BN_MARGIN_SIZE))>BN_MARGIN_SIZE)&&
        ((event.motion.y%(BN_TILE_SIZE + BN_MARGIN_SIZE))>BN_MARGIN_SIZE)
    )
    {
        switch (event.button.button)
        {
        case 1:
            if(!BN_getpos(board, rx, ry, BN_TYPE_SHOT))
            {
                pthread_mutex_lock(&(game->msgmutex));
                game->isTurn = 0;
                game->msg->type = BN_MSGTYPE_ACTION;
                game->msg->x = rx;
                game->msg->y = ry;
                pthread_mutex_unlock(&(game->msgmutex));
            }
            
            break;

        default:
            break;
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

void processInput(struct Game* game, struct BN_Board* board, struct BN_Board* board_self)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game->isRunning = 0;
            msg_pack msg = {BN_MSGTYPE_GAMEENDED, 0, 0};
            write(game->sd, &msg, sizeof(msg));
            break;

        
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_q)
            {
                game->isRunning = 0;
                msg_pack msg = {BN_MSGTYPE_GAMEENDED, 0, 0};
                write(game->sd, &msg, sizeof(msg));
            }
            else if(event.key.keysym.sym == SDLK_SPACE)
                BN_set_board(board, 0, 0);
            else if(event.key.keysym.sym == SDLK_w)
                BN_print_board(board);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(game->isTurn)
                handleMouseInput(game, event, board);
            break;
    
    }
}
void update(struct Game* game, struct BN_Board* board, struct BN_Board* board_self)
{

}
void render(struct Game* game, struct BN_Board* board, struct BN_Board* board_self)
{
    SDL_SetRenderDrawColor(game->renderer, BN_BLACK); // Negro
    SDL_RenderClear(game->renderer);
    

    SDL_Rect spacer ={BN_WINDOW_SIZE, 0, BN_BIG_MARGIN_SIZE, BN_WINDOW_SIZE};

    SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
    SDL_RenderFillRect(game->renderer, &spacer);    

    SDL_Rect rect ={0, 0, BN_TILE_SIZE, BN_TILE_SIZE};
    SDL_Rect dot ={0, 0, BN_DOT_SIZE, BN_DOT_SIZE};
    SDL_Rect ship ={0, 0, BN_SHIP_SIZE, BN_SHIP_SIZE};
    


    for (int y = 0; y < BN_COLUMNS; y++)
    {
        rect.y = BN_MARGIN_SIZE +  y*(BN_MARGIN_SIZE + BN_TILE_SIZE);
        ship.y = rect.y + BN_SHIP_MARGIN_SIZE;
        dot.y = rect.y + BN_DOT_MARGIN_SIZE;

        for (int x = 0; x < BN_COLUMNS; x++)
        {
            rect.x = BN_MARGIN_SIZE +  x*(BN_MARGIN_SIZE + BN_TILE_SIZE);

            SDL_RenderDrawRect(game->renderer, &rect);
            if(BN_getpos(board, x, y, BN_TYPE_SHIP) == 1)
            {
                ship.x = rect.x + BN_SHIP_MARGIN_SIZE;
                SDL_SetRenderDrawColor(game->renderer, BN_WHITE);
                SDL_RenderFillRect(game->renderer, &ship);
                SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
            }
            if(BN_getpos(board, x, y, BN_TYPE_SHOT) == 1)
            {
                dot.x = rect.x + BN_DOT_MARGIN_SIZE;
                SDL_SetRenderDrawColor(game->renderer, BN_RED);
                SDL_RenderFillRect(game->renderer, &dot);
                SDL_SetRenderDrawColor(game->renderer, BN_GRAY);
            }
        }
        
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
    SDL_RenderPresent(game->renderer);
}

void freeGame(struct Game* game)
{
    SDL_DestroyRenderer(game->renderer);
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