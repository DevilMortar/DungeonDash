#include "../kernel/header.h"

void saveData(SKIN *firstSkin, SAVE *dataSave, int skinNb){
    SKIN *tmp=firstSkin;
    FILE *saveFile;
    saveFile=fopen("saveFile.bin", "wba");
    if(saveFile==NULL){
        fprintf(stderr, "Error while opening file\n");
        exit(EXIT_FAILURE);
    }

    dataSave->skinState1=tmp->state;
    tmp=tmp->next;
    dataSave->skinState2=tmp->state;
    tmp=tmp->next;
    dataSave->skinState3=tmp->state;
    tmp=tmp->next;
    dataSave->skinState4=tmp->state;
    tmp=tmp->next;
    dataSave->skinState5=tmp->state;
    tmp=tmp->next;
    dataSave->skinState6=tmp->state;
    tmp=tmp->next;
    dataSave->skinState7=tmp->state;
    tmp=tmp->next;
    dataSave->skinState8=tmp->state;
    tmp=tmp->next;
    dataSave->skinState9=tmp->state;
    tmp=tmp->next;
    dataSave->skinState10=tmp->state;

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
        dataSave->skinState1=1;
        dataSave->skinState2=0;
        dataSave->skinState3=0;
        dataSave->skinState4=0;
        dataSave->skinState5=0;
        dataSave->skinState6=0;
        dataSave->skinState7=0;
        dataSave->skinState8=0;
        dataSave->skinState9=0;
        dataSave->skinState10=0;
    }
    else{
        fread(dataSave, sizeof(dataSave), 1, saveFile);
        fclose(saveFile);
    }
    return dataSave;
}