#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

// Display
SPRITE newSprite(SDL_Renderer *renderer, char link[255], int max, int srcsizew, int srcsizeh, int dstsize);
TEXTURE newTexture (SDL_Renderer *renderer, char link[255], int w, int h); // Renvoie une texture initialisée
void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int * frame); // Affiche un sprite et modifie sa frame
void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame); // Affiche un sprite
void updateSpriteIfNeeded(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame, int loop); // Affiche un sprite et modifie sa frame si besoin
TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height); // Affiche une texture
SDL_Texture* renderWidgetText(char *message, SDL_Color *color, int fontSize, SDL_Renderer *renderer, SDL_Rect * dstrect); // Renvoie le texte sous forme d'une texture
void displayText(SDL_Renderer *renderer, char *message, SDL_Color* color, int fontSize, SDL_Rect *dstrect); // Affiche un texte
void displayNumber(SDL_Renderer *renderer, int number, SDL_Color *color, int fontSize, SDL_Rect *dstrect); // Affiche un nombre 
void displayTextAndNumber(SDL_Renderer *renderer, char *message, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect); // Affiche un texte et un nombre
int numberOfDigit(int number); // Renvoie le nombre de chiffre d'un nombre