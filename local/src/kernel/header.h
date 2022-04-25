#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

typedef struct SONG SONG;
struct SONG
{
    Mix_Chunk * chunk;
    char * name;
    int channel;
    SONG * next;
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
    TEXTURE map;
    TEXTURE title;
    TEXTURE titleSkin;
    TEXTURE hole;
    TEXTURE gameOver;
    SPRITE deathAnimation;
    SPRITE scoreCoin;
    SDL_bool program_launched;
    SDL_bool game_launched;
    int score;
    int best;
    int money;
    unsigned int loop;
    int status;
    SDL_Rect endscorerect, endbestrect, coinrect;
};

typedef enum functions functions;
enum functions {play, skin, leave, confirm, right, left, locker, mainmenu, none, backToMenu};

typedef struct BUTTON BUTTON;
struct BUTTON{
    SPRITE button_sprite;
    enum functions function;
    int state;
    int menu;
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
int startGame(SDL_Window * window, SDL_Renderer *renderer, GAME * game, PLAYER * player, LIST_OBSTACLE fireball, int Hole[5][5],COIN * coin, SONG * songList, BUTTON * buttonList);

//Engine
void initGame(SDL_Renderer * renderer, GAME *game);
int init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer, GAME * game); // Initialisation des différents objets
PLAYER * setPlayer(int skin); // Set le player
POSITION randomTeleport(POSITION position, int Hole[5][5]); // Renvoie une position aléatoire où il n'y a pas de trous
void checkMovePlayer(PLAYER *player, int direction, int Hole[5][5]);
void movePlayer(PLAYER * player, int direction, int Hole[5][5]); // Déplace le joueur dans la direction "direction", si il n'y a pas de trous
LIST_OBSTACLE setListObstacle(LIST_OBSTACLE list); // Initialise les listes d'obstacles
LIST_OBSTACLE newObstacle(LIST_OBSTACLE list); // Créer un obstacle
LIST_OBSTACLE addToQueue(LIST_OBSTACLE list, OBSTACLE *new); // Ajouter un obstacle dans une file
LIST_OBSTACLE deleteFromQueue(LIST_OBSTACLE list); // Supprime un obstacle d'une file
int updateFireball(OBSTACLE *obstacle); // Déplace les boules de feu actives
bool detectColision(POSITION posplayer, POSITION posobject); // Détecte la colision 

// Display
SPRITE newSprite(SDL_Renderer *renderer, char link[255], int max, int srcsizew, int srcsizeh, int dstsize);
TEXTURE newTexture (SDL_Renderer *renderer, char link[255], int w, int h); // Renvoie une texture initialisée
void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int * frame); // Affiche un sprite et modifie sa frame
void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame); // Affiche un sprite
TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height); // Affiche une texture
void setPlayerSprite(SDL_Renderer *renderer, PLAYER *player, SKIN *skinList);
SDL_Texture* renderWidgetText(char *message, SDL_Color color, int fontSize, SDL_Renderer *renderer, SDL_Rect * dstrect); // Renvoie le texte sous forme d'une texture
int displayGame(SDL_Renderer *renderer, PLAYER *player, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], GAME *game, BUTTON * buttonList); // Affiche le jeu

// Menu
int menu(BUTTON * button, SKIN * skinList, SDL_Renderer *renderer, GAME *game, int *playerSkin); // Affiche le menu
void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, GAME * game); //Affiche le menu principal
void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, GAME * game); //Affiche le menu des skins
SKIN * createSkin(SDL_Renderer *renderer, char link[255], SKIN * skinList, int w, int h, int x, int y, int state, int price, int srcsizew, int srcsizeh); //Créé un nouveau skin
SKIN * addSkinInList(SKIN *skinList, SKIN *newSkin); //Ajoute un skin à la liste

//Buttons
BUTTON * createButton(SDL_Renderer *renderer, char link[255], BUTTON * buttonList, int w, int h, int x, int y, functions function,int state, int menu, int srcsizew, int srcsizeh); //Créé un nouveau bouton
BUTTON * addButtonInList(BUTTON * buttonList, BUTTON * newButton); //Ajoute un bouton à la liste des boutons
int checkClickButtons(BUTTON * buttonList, int options, int x, int y); //Vérifie si le click de la souris se fait dans un bouton
bool checkOverButtons(BUTTON * buttonList, int options, int x, int y); //Vérifie si la souris survole un bouton
void resetButtonState(BUTTON * buttonList); //Réinitialise l'état de tous les boutons à unclicked
void displayButtons(SDL_Renderer *renderer, BUTTON * buttonList, int options); //Affiche tous les boutons

//Son
SONG * loadSongInQueue(SONG * songList, char * path, char * name, int channel); //Charge une musique dans la file
void playSong(SONG * songList, char * name); //Joue la musique

//Data save
void saveData(SKIN *firstSkin, GAME *game);
void recupData(SKIN *firstSkin, GAME *game);
void resetData(SKIN *firstSkin, GAME *game);