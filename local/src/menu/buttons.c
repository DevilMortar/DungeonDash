#include "../kernel/header.h"


BUTTON * createButton(SDL_Renderer *renderer, char link[255], BUTTON * buttonList, int w, int h, int x, int y, functions function,int state, int menu, int srcsizew, int srcsizeh){
    BUTTON *new = malloc(sizeof(BUTTON));
    new->button_sprite=newSprite(renderer, link, 3, srcsizew, srcsizeh, 0);
    SDL_Rect dstrect = {x, y, w, h};
    new->button_sprite.dstrect=dstrect;
    new->function=function;
    new->state=state;
    new->menu=menu;
    new->next=NULL;
    return addButtonInList(buttonList, new);
}

BUTTON * addButtonInList(BUTTON * buttonList, BUTTON * newButton){
    if(buttonList != NULL && newButton !=NULL){
        newButton->next = buttonList;
        buttonList = newButton;
    }
    else{
        buttonList=newButton;
    }
    return buttonList;
}

int checkClickButtons(BUTTON * buttonList, int options, int menu, int x, int y){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if (tmp->menu==menu){
            SDL_Point mouse = {x, y};
            if(SDL_PointInRect(&mouse, &tmp->button_sprite.dstrect)){
                tmp->state=2;
                return tmp->function;
            }
        }
        tmp=tmp->next;
    }
    return options;
}

void checkOverButtons(BUTTON * buttonList, int options, int menu, int x, int y){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if (tmp->menu==menu){
            SDL_Point mouse = {x, y};
            if(SDL_PointInRect(&mouse, &tmp->button_sprite.dstrect)){
                tmp->state=3;
            }
        }
        tmp=tmp->next;
    }
}

void resetButtonState(BUTTON * buttonList){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        tmp->state=1;
        tmp=tmp->next;
    }
}

void displayButtons(SDL_Renderer *renderer, BUTTON * buttonList, int menu){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if (tmp->menu == menu){
            switch (tmp->state){
                case 1:
                    tmp->button_sprite.srcrect.x=0;
                    break;
                case 2:
                    tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
                    break;
                case 3:
                    tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew;
                    break;
            }
            SDL_RenderCopy(renderer, tmp->button_sprite.texture, &tmp->button_sprite.srcrect, &tmp->button_sprite.dstrect);
        }
        tmp=tmp->next;
    }
}