#include "sdl_funcs.h"
#include "BN.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <stdio.h>


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
            break;
        case SDL_MOUSEBUTTONDOWN:
            printf("%d\t%d\n", event.motion.xrel, event.motion.x);
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
