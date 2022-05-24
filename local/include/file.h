#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

/**
 * @brief Save game datas into a file
 * @param skinList
 * @param game
 */
void saveData(LIST_SKIN *skinList, GAME *game);

/**
 * @brief Get game datas from a file
 * @param skinList
 * @param game
 */
void recupData(LIST_SKIN *skinList, GAME *game);

/**
 * @brief Reset player's sprite with the default one
 * @param skinList
 * @param game
 */
void resetData(LIST_SKIN *skinList, GAME *game);

#endif