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

/**
 * @brief Define a new sprite
 * @param renderer - Renderer structure
 * @param link - Link to the sprite
 * @param max - Number of frame in the sprite
 * @param srcsizew - Width of the source rectangle
 * @param srcsizeh - Height of the source rectangle
 * @param dstsizew - Width of the destination rectangle
 * @param dstsizeh - Height of the destination rectangle
 * @return Sprite initialized
 */
SPRITE newSprite(SDL_Renderer *renderer, char link[255], int max, int srcsizew, int srcsizeh, int dstsizew, int dstsizeh);

/**
 * @brief Set a new texture
 * @param renderer - Renderer structure
 * @param link - Link to the texture
 * @param w - Width of the texture
 * @param h - Height of the texture
 * @return Texture initialized
 */
TEXTURE newTexture(SDL_Renderer *renderer, char link[255], int w, int h);

/**
 * @brief Update a sprite
 * @param renderer - Renderer structure
 * @param sprite - Sprite structure
 * @param direction - Direction of the sprite
 * @param position - Position structure
 * @param frame - Frame to apply for the skin
 */
void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame);

/**
 * @brief Display a sprite
 * @param renderer - Renderer structure
 * @param sprite - Sprite structure
 * @param direction - Direction of the sprite
 * @param position - Position structure of the sprite
 * @param frame - Frame of the sprite to display
 */
void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame);

/**
 * @brief Display a sprite and modify its frame if needed
 * @param renderer - Renderer structure
 * @param sprite - Sprite structure
 * @param direction - Direction of the sprite
 * @param position - Position structure of the sprite
 * @param frame - Frame of the sprite to display
 * @param loop - Current loop of the program
 */
void updateSpriteIfNeeded(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame, int loop);

/**
 * @brief Update a structure
 * @param renderer - Renderer structure
 * @param texture - Texture update
 * @param position - Position structure of the texture
 * @param width - Width of the structure
 * @param height - Height of the structure
 * @return The texture updated
 */
TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height);

/**
 * @brief Return the text in form of a texture
 * @param message - The message
 * @param color - The color of the message
 * @param fontSize - The size of the message
 * @param renderer - Renderer structure
 * @param dstrect - Destination rectangle
 * @return Text in form of a texture
 */
SDL_Texture *renderWidgetText(char *message, SDL_Color* color, int fontSize, SDL_Renderer *renderer, SDL_Rect *dstrect);
/**
 * @brief Display a number
 * @param renderer - Renderer structure
 * @param number - Number to display (should be int)
 * @param color - The color of the number
 * @param fontSize - The size of the number
 * @param dstrect - The destination rectangle
 */
void displayNumber(SDL_Renderer *renderer, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect);

/**
 * @brief Display Text and Number at the same time
 * @param renderer - Renderer structure
 * @param message - Message to display
 * @param number - Number to display
 * @param color - The color of the message and number
 * @param fontSize - The size of the text and number
 * @param dstrect - The destination rectangle
 */
void displayTextAndNumber(SDL_Renderer *renderer, char *message, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect);

/**
 * @brief Display a text
 * @param renderer - Renderer structure
 * @param message - Message to display
 * @param color - Color of message
 * @param fontSize - Size of the message
 * @param dstrect - The destination rectangle
 */
void displayText(SDL_Renderer *renderer, char *message, SDL_Color* color, int fontSize, SDL_Rect *dstrect);

/**
 * @brief Return the number of digit of a number
 * @param number - A number
 * @return The number of digit
 */
int numberOfDigit(int number);

/**
 * @brief Create a rectangle
 * @param x - The x position of the rectangle
 * @param y - The y position of the rectangle
 * @param w - The width of the rectangle
 * @param h - The height of the rectangle
 * @return - Rectangle structure
 */
SDL_Rect createRect(int x, int y, int w, int h);

// SDL
/**
 * @brief Exit the app with a message
 * @param message - Message to display
 */
void SDL_ExitWithError(const char * message);

/**
 * @brief Limit the FPS
 * @param limit - Number max of FPS
 */
void SDL_LimitFPS(unsigned int limit);

/**
 * @brief Initialization of the game view
 * @param window - Window structure
 * @param renderer - Renderer structure
 */
void SDL_initGameView(SDL_Window ** window, SDL_Renderer ** renderer);

#endif