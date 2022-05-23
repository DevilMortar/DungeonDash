#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "config.h"

typedef struct POSITION POSITION;
struct POSITION
{
    int x;
    int y;
    int direction;
};

typedef struct TEXTURE TEXTURE;
struct TEXTURE
{
    SDL_Texture *texture;
    SDL_Rect dstrect;
};

typedef struct SPRITE SPRITE;
struct SPRITE
{
    SDL_Texture *texture;
    SDL_Rect dstrect;
    SDL_Rect srcrect;
    int frame;
    int max;
    int srcsizew;
    int srcsizeh;
};

// Display
SPRITE newSprite(SDL_Renderer *renderer, char link[255], int max, int srcsizew, int srcsizeh, int dstsizew, int dstsizeh);
TEXTURE newTexture(SDL_Renderer *renderer, char link[255], int w, int h); // Renvoie une texture initialisée
void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame); // Affiche un sprite et modifie sa frame
void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame); // Affiche un sprite
void updateSpriteIfNeeded(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame, int loop); // Affiche un sprite et modifie sa frame si besoin
TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height); // Affiche une texture
SDL_Texture *renderWidgetText(char *message, SDL_Color* color, int fontSize, SDL_Renderer *renderer, SDL_Rect *dstrect); // Renvoie le texte sous forme d'une texture
void displayNumber(SDL_Renderer *renderer, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect); // Affiche un nombre
void displayTextAndNumber(SDL_Renderer *renderer, char *message, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect); // Affiche un texte et un nombre
void displayText(SDL_Renderer *renderer, char *message, SDL_Color* color, int fontSize, SDL_Rect *dstrect); // Affiche un texte
int numberOfDigit(int number); // Renvoie le nombre de chiffres d'un nombre
SDL_Rect createRect(int x, int y, int w, int h); // Renvoie un rectangle

// SDL
void SDL_ExitWithError(const char * message); // Quitter
void SDL_LimitFPS(unsigned int limit); // Limiter les FPS jusqu'à une certaine limite
void SDL_initGameView(SDL_Window ** window, SDL_Renderer ** renderer); // Initialiser la vue du jeu

#endif