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
#include "sound.h"

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

/**
 * @brief Initialization of the game
 * @param renderer - Renderer structure
 * @param game - Game structure
 */
void initGame(SDL_Renderer * renderer, GAME *game);

/**
 * @brief Initialization of various elements
 * @param player - Player structure
 * @param fireball - List of obstacles
 * @param Hole - Table with the coordinates of the holes
 * @param coin  - Coin structure
 * @param renderer - Renderer structure
 * @param game - Game structure
 */
void init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer, GAME * game);

/**
 * @brief Set player's skin
 * @param player - Player structure
 * @param skin - Skin structure
 */
void setPlayer(PLAYER * player, SKIN skin);

/**
 * @brief Reset skin size to the default one for the menu option
 * @param skin - Skin structure
 * @return Skin with right dimensions
 */
SKIN * resetSkinSize(SKIN * skin);

/**
 * @brief Return a position where objects can spawn
 * @param position - Position structure
 * @param Hole - Table with the holes coordinates
 * @return Position where objects can spawn
 */
POSITION randomTeleport(POSITION position, int Hole[5][5]);

/**
 * @brief Check if a player can move
 * @param player - Player structure
 * @param direction - Direction of where the player wants to go
 * @param Hole - Table with the holes coordinates
 * @return true if the user can move or false if not
 */
bool checkMovePlayer(PLAYER *player, int direction, int Hole[5][5]);

/**
 * @brief Move a player to the next case if possible
 * @param player - Player structure
 * @param direction - Direction of where the player wants to go
 * @param Hole - Table with the holes coordinates
 */
void movePlayer(PLAYER *player, int direction, int Hole[5][5]);

/**
 * @brief Initialization of the list of obstacles
 * @param list - List of obstacles
 */
void setListObstacle(LIST_OBSTACLE *list);

/**
 * @brief Create a new obstacle
 * @param list
 */
void newObstacle(LIST_OBSTACLE *list);

/**
 * @brief Add an obstacle to the list
 * @param list - List of obstacles
 * @param new - Obstacle to add
 */
void addToQueue(LIST_OBSTACLE *list, OBSTACLE *new);

/**
 * @brief Delete an obstacle from list
 * @param list - The list of obstacles
 */
void deleteFromQueue(LIST_OBSTACLE *list);

/**
 * @brief Update fireball
 * @param obstacle - The fireball to move
 * @return Callable for future use
 */
int updateFireball(OBSTACLE *obstacle);

/**
 * @brief Distance between two positions
 * @param pos1 - Position structure of object 1
 * @param pos2  - Position structure of object 2
 * @return The distance between these two positions
 */
int distance(POSITION pos1, POSITION pos2);

/**
 * @brief Detect if a collision occurred
 * @param pos1 - Position structure of object 1
 * @param pos2 - Position strcuture of object 2
 * @return true if collision occurred, else false
 */
bool detectColision(POSITION pos1, POSITION pos2);

/**
 * @brief Set the player's sprite
 * @param player - Player structure
 * @param skin - Skin structure
 */
void setPlayerSprite(PLAYER *player, SKIN * skin);

#endif