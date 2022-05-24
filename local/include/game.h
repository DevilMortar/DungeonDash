#ifndef GAME_H
#define GAME_H

#include <math.h>
#include "button.h"
#include "config.h"
#include "engine.h"
#include "sound.h"

/**
 * @brief Start the game
 * @param window - Window structure
 * @param renderer - Renderer structure
 * @param game - Game structure
 * @param player - Player structure
 * @param fireball - List of obstacles
 * @param Hole - Table with the holes coordinates
 * @param coin - Coin structure
 * @param buttonList - List of buttons
 * @return Positive number if ready to launch, else negative number
 */
int startGame(SDL_Window *window, SDL_Renderer *renderer, GAME *game, PLAYER *player, LIST_OBSTACLE fireball, int Hole[5][5], COIN *coin, BUTTON *buttonList);

/**
 * @brief Display the game
 * @param renderer - Renderer structure
 * @param player - Player structure
 * @param fireball - list of obstacles
 * @param coin - Coin structure
 * @param Hole - Table with the holes coordinates
 * @param game - Game structure
 * @param buttonList - List of buttons
 * @return Positive number if available for show, else negative value
 */
int displayGame(SDL_Renderer *renderer, PLAYER *player, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], GAME *game, BUTTON *buttonList);

#endif