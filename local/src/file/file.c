#include "../kernel/header.h"

void saveData(SKIN *firstSkin, GAME *game){
    FILE *saveFile;
    SKIN *tmp=firstSkin;
    saveFile=fopen("saveFile.bin" ,"wba");
    if(saveFile==NULL){
        printf("Error while trying to open saveFile.bin\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&game->money, sizeof(int), 1, saveFile);
    fwrite(&game->best, sizeof(int), 1, saveFile);
    fwrite(&tmp->state, sizeof(int), 1, saveFile);
    tmp=tmp->next;
    for(int i=0; i<9; i++)
    {
        fwrite(&tmp->state, sizeof(int), 1, saveFile);
        tmp=tmp->next;
    }
    fclose(saveFile);
}

void recupData(SKIN *firstSkin, GAME *game){
    FILE *saveFile;
    SKIN *tmp=firstSkin;
    saveFile=fopen("saveFile.bin" ,"rb");
    if(saveFile==NULL){
        game->money=0;
        game->best=0;
    }
    else{
        fread(&game->money, sizeof(int), 1, saveFile);
        fread(&game->best, sizeof(int), 1, saveFile);
        fread(&tmp->state, sizeof(int), 1, saveFile);
        tmp=tmp->next;
        for(int i=0; i<9; i++){
            fread(&tmp->state, sizeof(int), 1, saveFile);
            tmp=tmp->next;
        }
    }
    fclose(saveFile);
}