#ifndef SDL_BN_lib
#define SDL_BN_lib

#include <SDL2/SDL.h>
#include <pthread.h>
#include "BN.h"

#define DEFAULT_ERROR -1
// BN: batalla naval
#define BN_COLUMNS 8

#define BN_MARGIN_SIZE 5
#define BN_TILE_SIZE 16
#define BN_BIG_MARGIN_SIZE 10
#define BN_SPACE_SIZE 5
#define BN_DOT_SIZE 4
#define BN_SHIP_SIZE 10
#define BN_SHIP_DOWN_OFFSET_SIZE 2

#define BN_DOT_MARGIN_SIZE ((BN_TILE_SIZE - BN_DOT_SIZE)/2)
#define BN_SHIP_MARGIN_SIZE ((BN_TILE_SIZE - BN_SHIP_SIZE)/2)

#define BN_WINDOW_SIZE (BN_COLUMNS * (BN_TILE_SIZE + BN_MARGIN_SIZE) + BN_MARGIN_SIZE)

#define BN_BLACK 0, 0, 0, 255
#define BN_GREEN 0, 255, 0, 255
#define BN_RED 255, 64, 0, 255
#define BN_BLUE 150, 180, 255, 255
#define BN_DGRAY 80, 80, 80, 255
#define BN_GRAY 180, 180, 180, 255
#define BN_YLW 255, 130, 30, 255
#define BN_WHITE 255, 255, 255, 255
#define BN_ORG 180, 130, 0, 255

typedef struct Node {
    SDL_Rect rect;
    struct Node* next;
} Node;

typedef struct Game
{
    SDL_Window * win;
    SDL_Renderer * renderer;

    unsigned char isRunning:1;
    unsigned char isTurn:1;
    unsigned char isWon:1;
    unsigned char threadEnded:1;

    msg_pack* msg;
    int sd;
    pthread_mutex_t msgmutex;
} Game;

int initializeGame(struct Game*);

void handleMouseInput(struct Game* game, SDL_Event event, struct BN_Board* board);
void processInput(struct Game*, struct BN_Board*, struct BN_Board* self);
void update(struct Game*, struct BN_Board*, struct BN_Board* self);
void render(struct Game*, struct BN_Board*, struct BN_Board* self);

void endGame(struct Game* game);
void freeGame(struct Game*);

#endif