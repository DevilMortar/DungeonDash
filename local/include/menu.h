#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL_render.h>
#include "button.h"
#include "display.h"
#include "engine.h"
#include "file.h"
#include "sound.h"

/**
 * @brief Display the start menu
 * @param button - List of buttons
 * @param skinList - List of skin
 * @param player - Player structure
 * @param renderer - Renderer structure
 * @param game - Game structure
 * @return Number for action to do
 */
int startMenu(BUTTON * button, LIST_SKIN *skinList, PLAYER *player, SDL_Renderer *renderer, GAME *game);

/**
 * @brief Display main menu
 * @param buttonList - List of buttons
 * @param skinList - List of skins
 * @param renderer - Renderer structure
 * @param game - Game structure
 */
void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, GAME * game);

/**
 * @brief Display skin menu
 * @param buttonList - List of buttons
 * @param skinListTMP - List of skins
 * @param renderer - Renderer structure
 * @param game - Game structure
 */
void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, GAME * game);

/**
 * @brief Create a skin
 * @param renderer - Renderer structure
 * @param link - Link for the skin
 * @param skinList - List of skins
 * @param state - State of the skin
 * @param price - Price of the skin
 * @param srcsizew - Width of the sprite
 * @param srcsizeh - Height of the sprite
 * @return List of skins
 */
LIST_SKIN *createSkin(SDL_Renderer *renderer, char *link, LIST_SKIN *skinList, int state, int price, int srcsizew, int srcsizeh);

/**
 * @brief Add a skin in the list
 * @param skinList - List of skins
 * @param newSkin - Skin
 * @return List of skin
 */
LIST_SKIN *addSkinInList(LIST_SKIN *skinList, SKIN *newSkin);

/**
 * @brief Browse list of skin
 * @param tmp - Skin to start search from
 * @param direction - Direction of the skin
 * @param best - The best score of the player
 * @return Skin
 */
SKIN *browseSkin(SKIN *tmp, int direction, int best);

/**
 * @brief Free list of skin
 * @param skinList - List of skins
 */
void freeSkinList(LIST_SKIN *skinList);

/**
 * @brief Play intro
 * @param renderer - Renderer structure
 * @param game - Game structure
 */
void playIntro(SDL_Renderer *renderer, GAME *game);

#endif
