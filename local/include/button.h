#ifndef BUTTON_H
#define BUTTON_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL_render.h>
#include "config.h"
#include "display.h"
#include "engine.h"

typedef enum functions functions;
enum functions {play, skin, leave, confirm, right, left, locker, backToMenu, reset, sound, none};

typedef enum button_state button_state;
enum button_state {locked, unlocked};

typedef struct BUTTON BUTTON;
struct BUTTON{
    SPRITE button_sprite;
    enum functions function;
    enum button_state state;
    enum menu menu;
    BUTTON * next;
};

BUTTON *createButton(SDL_Renderer *renderer, char *link, BUTTON * buttonList, int w, int h, int x, int y, functions function, menu menu, int srcsizew, int srcsizeh); //Créé un nouveau bouton
BUTTON *addButtonInList(BUTTON * buttonList, BUTTON * newButton); //Ajoute un bouton à la liste des boutons
BUTTON *checkClickButtons(BUTTON * buttonList, enum functions * options, int menu, int x, int y); //Vérifie si le click de la souris se fait dans un bouton
void checkOverButtons(BUTTON * buttonList, int options, int menu, int x, int y); //Vérifie si la souris survole un bouton
void resetButtonState(BUTTON * buttonList); //Réinitialise l'état de tous les boutons à unclicked
void displayButton(SDL_Renderer *renderer, BUTTON * button); //Affiche un bouton
void displayButtonList(SDL_Renderer *renderer, BUTTON * buttonList, int menu); //Affiche la liste des boutons
void buttonChangeState(BUTTON * button, int state); //Change l'état d'un bouton
void freeButtons(BUTTON * buttonList); //Libère la mémoire allouée pour les boutons

#endif