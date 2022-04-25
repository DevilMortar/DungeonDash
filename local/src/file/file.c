#include "../kernel/header.h"

void saveData(SKIN *firstSkin, GAME *game){
    FILE *saveFile=fopen("saveFile.bin" ,"wba");
    SKIN *tmp=firstSkin;
    if(saveFile==NULL){
        printf("Error while trying to open saveFile.bin\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&game->money, sizeof(int), 1, saveFile);
    fwrite(&game->best, sizeof(int), 1, saveFile);
    while(tmp!=NULL){
        fwrite(&tmp->state, sizeof(int), 1, saveFile);
        tmp=tmp->next;
    }
    fclose(saveFile);
}

void recupData(SKIN *firstSkin, GAME *game){
    FILE *saveFile=fopen("saveFile.bin" ,"rb");
    SKIN *tmp=firstSkin;
    if(saveFile==NULL){
        printf("Error while trying to open saveFile.bin\n");
        exit(EXIT_FAILURE);
    }
    else{
        fread(&game->money, sizeof(int), 1, saveFile);
        fread(&game->best, sizeof(int), 1, saveFile);
        while(tmp!=NULL){
            fread(&tmp->state, sizeof(int), 1, saveFile);
            tmp=tmp->next;
        }
    }
    fclose(saveFile);
}

void resetData(SKIN *firstSkin, GAME *game){
    game->money=0;
    game->best=0;
    SKIN *tmp=firstSkin;
    tmp=tmp->next;
    while(tmp!=NULL){
        tmp->state=0;
        tmp=tmp->next;
    }
    saveData(firstSkin, game);
}