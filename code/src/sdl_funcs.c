#include "server_funcs.h"
#include "sdl_funcs.h"
#include "BN.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <linux/limits.h>
#include <stdlib.h>
//#include <stdio.h>


void handleMouseInput(SDL_Event event, struct BN_Board* board)
{
    int rx, ry;
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
            BN_setpos(board, rx, ry, BN_TYPE_SHIP, 1);
            break;
        case 2:
            printf("%d\n", BN_checkship(board, rx, ry, 0));
            break;
        case 3:
            BN_setpos(board, rx, ry, BN_TYPE_SHOT, 1);
            break;
        default:
            printf("%d\n",event.button.button);
            break;
        }
    }

    

    return;
}

int initializeWindow(struct Game* game)
{
    int errcode = SDL_Init(SDL_INIT_EVERYTHING);
    game->isRunning = 1;
    if(errcode == 0)
    {
        game->win = SDL_CreateWindow( //Creando el "objeto" (como puntero a struct) de la ventana
            "Game",
            SDL_WINDOWPOS_CENTERED, // La libreria se fija para centrarlo 
            SDL_WINDOWPOS_CENTERED,
            BN_WINDOW_SIZE,
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

void processInput(struct Game* game, struct BN_Board* board)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game->isRunning = 0;
            break;

        
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_q)
                game->isRunning = 0;
            else if(event.key.keysym.sym == SDLK_SPACE)
                BN_set_board(board, 0, 0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            handleMouseInput(event, board);
            break;
    
    }
}
void update(struct Game* game, struct BN_Board* board)
{

}
void render(struct Game* game, struct BN_Board* board)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    
    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);

    SDL_Rect rect ={0, 0, BN_TILE_SIZE, BN_TILE_SIZE};
    SDL_Rect dot ={0, 0, BN_DOT_SIZE, BN_DOT_SIZE};
    SDL_Rect ship ={0, 0, BN_SHIP_SIZE, BN_SHIP_SIZE};
    
    for (int x = 0; x < BN_COLUMNS; x++)
    {
        rect.x = BN_MARGIN_SIZE +  x*(BN_MARGIN_SIZE + BN_TILE_SIZE);
        dot.x = rect.x + BN_DOT_MARGIN_SIZE;
        ship.x = rect.x + BN_SHIP_MARGIN_SIZE;

        for (int y = 0; y < BN_COLUMNS; y++)
        {
            rect.y = BN_MARGIN_SIZE +  y*(BN_MARGIN_SIZE + BN_TILE_SIZE);

            SDL_RenderDrawRect(game->renderer, &rect);
            if(BN_getpos(board, x, y, BN_TYPE_SHIP) == 1)
            {
                SDL_SetRenderDrawColor(game->renderer, 255, 64, 0, 255);
                ship.y = rect.y + BN_SHIP_MARGIN_SIZE;
                SDL_RenderFillRect(game->renderer, &ship);
                SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
            }
            if(BN_getpos(board, x, y, BN_TYPE_SHOT) == 1)
            {
                dot.y = rect.y + BN_DOT_MARGIN_SIZE;
                SDL_RenderFillRect(game->renderer, &dot);
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
