#include "../display/display.h"

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
bool updateFireball(OBSTACLE *obstacle); // Déplace les boules de feu actives
bool detectColision(POSITION posplayer, POSITION posobject); // Détecte la colision 

