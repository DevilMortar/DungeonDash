#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL_render.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "config.h"
#include "display.h"

typedef enum menu menu;
enum menu {mainMenu, skinMenu, gameOverMenu, gameLaunched};

typedef enum skin_state skin_state;
enum skin_state {available = -1, tobeUnlocked = 0, selected = 1};

typedef struct SKIN SKIN;
struct SKIN{
    SPRITE skin_sprite;
    enum skin_state state;
    int price;
    SKIN *previous;
    SKIN *next;
};

typedef struct LIST_SKIN LIST_SKIN;
struct LIST_SKIN{
    SKIN *first;
    SKIN *last;
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
    OBSTACLE *first;
    OBSTACLE *last;
    SPRITE sprite;
    SPRITE warning;
};

typedef struct GAME GAME;
struct GAME
{
    TEXTURE background;
    TEXTURE endscreen;
    TEXTURE map;
    TEXTURE title;
    TEXTURE titleSkin;
    TEXTURE hole;
    TEXTURE gameOver;
    SPRITE deathAnimation;
    SPRITE scoreCoin;
    SDL_bool program_launched;
    SDL_bool game_launched;
    enum menu menu;
    int score;
    int best;
    int money;
    unsigned int loop;
    int status;
    SDL_Rect endscorerect, endbestrect, coinrect;
};

typedef struct PLAYER PLAYER;
struct PLAYER
{
    POSITION position;
    SKIN skin;
    int move;
    int directionPressed;
};

typedef struct COIN COIN;
struct COIN
{
    POSITION position;
    SPRITE sprite;
};

void initGame(SDL_Renderer * renderer, GAME *game);
int init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer, GAME * game); // Initialisation des différents objets
void setPlayer(PLAYER * player, SKIN skin); // Set le player
SKIN resetSkinSize(SKIN skin); // Redéfinis les dimensions du skin à la taille d'origine
POSITION randomTeleport(POSITION position, int Hole[5][5]); // Renvoie une position aléatoire où il n'y a pas de trous
bool checkMovePlayer(PLAYER *player, int direction, int Hole[5][5]);
void movePlayer(PLAYER *player, int direction, int Hole[5][5]); // Déplace le joueur dans la direction "direction", s'il n'y a pas de trous
void setListObstacle(LIST_OBSTACLE *list); // Initialise les listes d'obstacles
void newObstacle(LIST_OBSTACLE *list); // Créer un obstacle
void addToQueue(LIST_OBSTACLE *list, OBSTACLE *new); // Ajouter un obstacle dans une file
void deleteFromQueue(LIST_OBSTACLE *list); // Supprime un obstacle d'une file
int updateFireball(OBSTACLE *obstacle); // Déplace les boules de feu actives
int distance(POSITION pos1, POSITION pos2); // Renvoie la distance entre deux positions
bool detectColision(POSITION pos1, POSITION pos2); // Détecte la collision
void setPlayerSprite(PLAYER *player, SKIN skin); // Set le sprite du joueur

#endif