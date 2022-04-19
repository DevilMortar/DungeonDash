#include "../kernel/header.h"

void saveData(SKIN *firstSkin, SAVE *dataSave, int skinNb){
    SKIN *tmp=firstSkin;
    FILE *saveFile;
    saveFile=fopen("saveFile.bin", "wba");
    if(saveFile==NULL){
        fprintf(stderr, "Error while opening file\n");
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<skinNb; i++){
        dataSave->skinState[i]=tmp->state;
        tmp=tmp->next;
    }

    fwrite(dataSave, sizeof(dataSave), 1, saveFile);
    fclose(saveFile);
}

SAVE * recupData(int skinNb){
    SAVE *dataSave=NULL;
    dataSave=malloc(sizeof(SAVE));
    FILE *saveFile;
    saveFile=fopen("saveFile.bin", "rb");
    if(saveFile==NULL){
        dataSave->money=0;
        dataSave->highscore=0;
        dataSave->skinState[0]=1;
        for(int i=1; i<skinNb; i++)
            dataSave->skinState[i]=0;
    }
    else{
        fread(dataSave, sizeof(dataSave), 1, saveFile);
        fclose(saveFile);
    }
    return dataSave;
}