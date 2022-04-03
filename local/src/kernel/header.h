#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"

typedef struct TEXTURE TEXTURE;
struct TEXTURE
{
    SDL_Texture * texture;
    SDL_Rect dstrect;
};

typedef struct SPRITE SPRITE;
struct SPRITE
{
    SDL_Texture * texture;
    SDL_Rect dstrect;
    SDL_Rect srcrect;
    int frame;
    int max;
    int srcsizew;
    int srcsizeh;
};

typedef struct POSITION POSITION;
struct POSITION
{
    int x;
    int y;
    int direction;
};

typedef struct OBSTACLE OBSTACLE;
struct OBSTACLE
{
    POSITION position;
    OBSTACLE * previous;
    OBSTACLE * next;
    int frame;
    int warning;
};

typedef struct LIST_OBSTACLE LIST_OBSTACLE;
struct LIST_OBSTACLE
{
    OBSTACLE * first;
    OBSTACLE * last;
    SPRITE sprite;
    SPRITE warning;
};

typedef struct PLAYER PLAYER;
struct PLAYER
{
    POSITION position;
    SPRITE sprite;
    int skin;
    int move; 
};

typedef struct COIN COIN;
struct COIN
{
    POSITION position;
    SPRITE sprite;  
};

typedef struct GAME GAME;
struct GAME
{
    TEXTURE endscreen;
    SPRITE deathAnimation;
    SPRITE scoreCoin;
    int score;
    int best;
    unsigned int loop;
    int status;
    SDL_Rect endscorerect, endbestrect, coinrect;
};

// SDL
void SDL_ExitWithError(const char * message); // Quitter
int startGame(SDL_Window * window, SDL_Renderer *renderer, GAME * game, PLAYER * player, LIST_OBSTACLE fireball, int Hole[5][5],COIN * coin, TEXTURE map, TEXTURE hole);





