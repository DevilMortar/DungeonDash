#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
#define FPS_DELAY 16
#define ANIMATION_LOOP 4
#define CASE_SIZE 71
#define CASE_OFFSET_X 263
#define CASE_OFFSET_Y 11
#define SPRITE_SIZE 50
#define FIREBALL_SPEED 20

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
    int srcsize;
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
    
};

typedef struct COIN COIN;
struct COIN
{
    POSITION position;
    SPRITE sprite;  
};

//Engine
PLAYER * setPlayer(); // Set le player
POSITION randomTeleport(POSITION position, int Hole[7][7]); // Renvoie une position aléatoire où il n'y a pas de trous
void movePlayer(PLAYER * player, int direction, int Hole[7][7]); // Déplace le joueur dans la direction "direction", si il n'y a pas de trous
LIST_OBSTACLE setListObstacle(LIST_OBSTACLE list); // Initialise les listes d'obstacles
LIST_OBSTACLE newObstacle(LIST_OBSTACLE list); // Créer un obstacle
LIST_OBSTACLE addToQueue(LIST_OBSTACLE list, OBSTACLE *new); // Ajouter un obstacle dans une file
LIST_OBSTACLE deleteFromQueue(LIST_OBSTACLE list); // Supprime un obstacle d'une file
LIST_OBSTACLE updateFireball(LIST_OBSTACLE fireball); // Déplace les boules de feu actives
bool detectColision(POSITION posplayer, POSITION posobject); // Détecte la colision 


// SDL
void SDL_ExitWithError(const char * message); // Quitter 

// Display
SPRITE newSprite(SDL_Renderer *renderer, char lien[255], int frame, int srcsize, int dstsize); // Renvoie un sprite initialisée
TEXTURE newTexture (SDL_Renderer *renderer, char lien[255]); // Renvoie une texture initialisée
void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int * frame); // Affiche un sprite et modifie sa frame
void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame); // Affiche un sprite
TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height); // Affiche une texture
void setPlayerSprite(SDL_Renderer * renderer, PLAYER * player); // Définit le sprite du joueur en fonction du skin
int displayGame(SDL_Renderer *renderer, PLAYER *player, TEXTURE map, LIST_OBSTACLE fireball, LIST_OBSTACLE laser, COIN *coin, int Hole[7][7], TEXTURE hole, int loop); // Affiche le jeu





