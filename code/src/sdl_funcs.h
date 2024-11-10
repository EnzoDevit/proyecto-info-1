#ifndef SDL_BN_lib
#define SDL_BN_lib

#include <SDL2/SDL.h>
#include <SDL2/SDL_shape.h>
#include <SDL2/SDL_video.h>
#include "BN.h"

#define DEFAULT_ERROR -1
// BN: batalla naval
#define BN_MARGIN_SIZE 5
#define BN_TILE_SIZE 60
#define BN_COLUMNS 8

#define BN_BIG_MARGIN_SIZE 15

#define BN_SPACE_SIZE 5

#define BN_DOT_SIZE 10
#define BN_SHIP_SIZE 40

#define BN_DOT_MARGIN_SIZE ((BN_TILE_SIZE - BN_DOT_SIZE)/2)
#define BN_SHIP_MARGIN_SIZE ((BN_TILE_SIZE - BN_SHIP_SIZE)/2)

#define BN_WINDOW_SIZE (BN_COLUMNS * (BN_TILE_SIZE + BN_MARGIN_SIZE) + BN_MARGIN_SIZE)

#define BN_BLACK 0, 0, 0, 255
#define BN_GREEN 0, 255, 0, 255
#define BN_RED 255, 64, 0, 255
#define BN_BLUE 64, 64, 255, 255
#define BN_GRAY 180, 180, 180, 255


struct Game
{
    SDL_Window * win;
    SDL_Renderer * renderer;
    unsigned int isRunning:1;
};

int initializeWindow(struct Game*);

void processInput(struct Game*, struct BN_Board*, struct BN_Board* self);
void update(struct Game*, struct BN_Board*, struct BN_Board* self);
void render(struct Game*, struct BN_Board*, struct BN_Board* self);

void freeGame(struct Game*);

#endif