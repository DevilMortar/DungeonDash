#ifndef GAME_H
#define GAME_H

#include <math.h>
#include "button.h"
#include "config.h"
#include "engine.h"
#include "sound.h"

int startGame(SDL_Window *window, SDL_Renderer *renderer, GAME *game, PLAYER *player, LIST_OBSTACLE fireball, int Hole[5][5], COIN *coin, BUTTON *buttonList);
int displayGame(SDL_Renderer *renderer, PLAYER *player, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], GAME *game, BUTTON *buttonList);

#endif