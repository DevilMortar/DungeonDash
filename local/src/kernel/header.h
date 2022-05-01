#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "config.h"
#include "../soundLib/sound.h"
#include "../display/display.h"

typedef enum menu menu;
enum menu {mainMenu, skinMenu, gameOverMenu, gameLaunched};

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
    int directionPressed; 
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

typedef enum functions functions;
enum functions {play, skin, leave, confirm, right, left, locker, backToMenu, reset, sound, none};

typedef struct BUTTON BUTTON;
struct BUTTON{
    SPRITE button_sprite;
    enum functions function;
    int state;
    int menu;
    bool preventReset;
    BUTTON * next;
};

typedef struct SKIN SKIN;
struct SKIN{
    SPRITE skin_sprite;
    int state;
    int price;
    SKIN * previous;
    SKIN * next;
};

// SDL
void SDL_ExitWithError(const char * message); // Quitter
void SDL_LimitFPS(unsigned int limit);
void SDL_initGameView(SDL_Window ** window, SDL_Renderer ** renderer);
int startGame(SDL_Window * window, SDL_Renderer *renderer, GAME * game, PLAYER * player, LIST_OBSTACLE fireball, int Hole[5][5],COIN * coin, SL_SOUND * soundList, BUTTON * buttonList);
int displayGame(SDL_Renderer *renderer, PLAYER *player, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], GAME *game, BUTTON * buttonList); // Affiche le jeu

//Engine
void initGame(SDL_Renderer * renderer, GAME *game);
int init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer, GAME * game); // Initialisation des différents objets
PLAYER * setPlayer(int skin); // Set le player
POSITION randomTeleport(POSITION position, int Hole[5][5]); // Renvoie une position aléatoire où il n'y a pas de trous
bool checkMovePlayer(PLAYER *player, int direction, int Hole[5][5]);
void movePlayer(PLAYER * player, int direction, int Hole[5][5]); // Déplace le joueur dans la direction "direction", si il n'y a pas de trous
LIST_OBSTACLE setListObstacle(LIST_OBSTACLE list); // Initialise les listes d'obstacles
LIST_OBSTACLE newObstacle(LIST_OBSTACLE list); // Créer un obstacle
LIST_OBSTACLE addToQueue(LIST_OBSTACLE list, OBSTACLE *new); // Ajouter un obstacle dans une file
LIST_OBSTACLE deleteFromQueue(LIST_OBSTACLE list); // Supprime un obstacle d'une file
int updateFireball(OBSTACLE *obstacle); // Déplace les boules de feu actives
int distance(POSITION pos1, POSITION pos2); // Renvoie la distance entre deux positions
bool detectColision(POSITION pos1, POSITION pos2); // Détecte la colision 
void setPlayerSprite(SDL_Renderer *renderer, PLAYER *player, SKIN *skinList); // Set le sprite du joueur

// Menu
int startMenu(BUTTON * button, SKIN * skinList, SKIN * firstSkin, SDL_Renderer *renderer, GAME *game, int *playerSkin); // Affiche le menu
void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, GAME * game); //Affiche le menu principal
void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, GAME * game); //Affiche le menu des skins
SKIN * createSkin(SDL_Renderer *renderer, char link[255], SKIN * skinList, int w, int h, int x, int y, int state, int price, int srcsizew, int srcsizeh); //Créé un nouveau skin
SKIN * addSkinInList(SKIN *skinList, SKIN *newSkin); //Ajoute un skin à la liste
SKIN * browseSkin(SKIN *tmp, int direction, int best); //Parcours la liste de skin
void freeSkinList(SKIN *skinList); //Libère la liste de skin

//Buttons
BUTTON * createButton(SDL_Renderer *renderer, char link[255], BUTTON * buttonList, int w, int h, int x, int y, functions function, int menu, int srcsizew, int srcsizeh, bool preventReset); //Créé un nouveau bouton
BUTTON * addButtonInList(BUTTON * buttonList, BUTTON * newButton); //Ajoute un bouton à la liste des boutons
BUTTON * checkClickButtons(BUTTON * buttonList, int *options, int menu, int x, int y); //Vérifie si le click de la souris se fait dans un bouton
void checkOverButtons(BUTTON * buttonList, int options, int menu, int x, int y); //Vérifie si la souris survole un bouton
void resetButtonState(BUTTON * buttonList); //Réinitialise l'état de tous les boutons à unclicked
void displayButton(SDL_Renderer *renderer, BUTTON * button); //Affiche un bouton
void displayButtonList(SDL_Renderer *renderer, BUTTON * buttonList, int menu); //Affiche la liste des boutons
void buttonChangeState(BUTTON * button, int state); //Change l'état d'un bouton
void freeButtons(BUTTON * buttonList); //Libère la mémoire allouée pour les boutons

//Data save
void saveData(SKIN *firstSkin, GAME *game);
void recupData(SKIN *firstSkin, GAME *game);
void resetData(SKIN *firstSkin, GAME *game);