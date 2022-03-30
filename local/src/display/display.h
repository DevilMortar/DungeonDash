#include "../kernel/header.h"

// Display
SPRITE newSprite(SDL_Renderer *renderer, char link[255], int max, int srcsizew, int srcsizeh, int dstsize);
TEXTURE newTexture (SDL_Renderer *renderer, char link[255], int w, int h); // Renvoie une texture initialisée
void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int * frame); // Affiche un sprite et modifie sa frame
void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame); // Affiche un sprite
TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height); // Affiche une texture
void setPlayerSprite(SDL_Renderer * renderer, PLAYER * player); // Définit le sprite du joueur en fonction du skin
SDL_Texture* renderWidgetText(char *message, SDL_Color color, int fontSize, SDL_Renderer *renderer, SDL_Rect * dstrect); // Renvoie le texte sous forme d'une texture
int displayGame(SDL_Renderer *renderer, PLAYER *player, TEXTURE map, LIST_OBSTACLE fireball, COIN *coin, int Hole[7][7], TEXTURE hole, int loop, END *end, int score); // Affiche le jeu
void SDL_LimitFPS(unsigned int limit);