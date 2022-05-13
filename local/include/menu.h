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

int startMenu(BUTTON * button, LIST_SKIN *skinList, PLAYER *player, SDL_Renderer *renderer, GAME *game); // Affiche le menu
void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, GAME * game); //Affiche le menu principal
void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, GAME * game); //Affiche le menu des skins
LIST_SKIN *createSkin(SDL_Renderer *renderer, char *link, LIST_SKIN *skinList, int state, int price, int srcsizew, int srcsizeh); //Créé un nouveau skin
LIST_SKIN *addSkinInList(LIST_SKIN *skinList, SKIN *newSkin); //Ajoute un skin à la liste
SKIN *browseSkin(SKIN *tmp, int direction, int best); //Parcours la liste de skin
void freeSkinList(LIST_SKIN *skinList); //Libère la liste de skin

#endif
