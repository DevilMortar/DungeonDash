#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

void saveData(LIST_SKIN *skinList, GAME *game); // Sauvegarde les données du jeu
void recupData(LIST_SKIN *skinList, GAME *game); // Récupère les données du jeu depuis la sauvegarde
void resetData(LIST_SKIN *skinList, GAME *game); // Réinitialise le sprite du joueur par celui par défaut

#endif