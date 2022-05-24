#include "../include/file.h"

void saveData(LIST_SKIN *skinList, GAME *game) {
    FILE *saveFile = fopen("../bin/saveFile.bin" ,"wb");
    SKIN *tmp = skinList->first;
    if(saveFile == NULL){
        printf("Error while trying to open saveFile.bin\n");
        exit(EXIT_FAILURE);
    } else {
        fwrite(&game->money, sizeof(int), 1, saveFile);
        fwrite(&game->best, sizeof(int), 1, saveFile);
        while(tmp != NULL){
            fwrite(&tmp->state, sizeof(int), 1, saveFile);
            tmp = tmp->next;
        }
        fclose(saveFile);
    }
}

void recupData(LIST_SKIN *skinList, GAME *game) {
    FILE *saveFile = fopen("../bin/saveFile.bin" ,"rb");
    SKIN *tmp = skinList->first;
    if(saveFile == NULL) {
        printf("Error while trying to open saveFile.bin\n");
        exit(EXIT_FAILURE);
    } else {
        fread(&game->money, sizeof(int), 1, saveFile);
        fread(&game->best, sizeof(int), 1, saveFile);
        while(tmp != NULL) {
            fread(&tmp->state, sizeof(int), 1, saveFile);
            tmp = tmp->next;
        }
        fclose(saveFile);
    }
}

void resetData(LIST_SKIN *skinList, GAME *game) {
    game->money = 0;
    game->best = 0;
    SKIN *tmp = skinList->first;
    while(tmp!=NULL) {
        tmp->state = 0;
        tmp = tmp->next;
    }
    saveData(skinList, game);
}