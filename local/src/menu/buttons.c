#include "../kernel/header.h"


BUTTON * createButton(SDL_Renderer *renderer, char link[255], BUTTON * buttonList, int w, int h, int x, int y, functions function,int state, int menu, int srcsizew, int srcsizeh){
    BUTTON *new = malloc(sizeof(BUTTON));
    new->button_sprite=newSprite(renderer, link, 3, srcsizew, srcsizeh, 0);
    new->button_sprite.dstrect.w=w;
    new->button_sprite.dstrect.h=h;
    new->button_sprite.dstrect.x=x;
    new->button_sprite.dstrect.y=y;
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

int checkClickButtons(BUTTON * buttonList, SKIN *skinListTMP, int options, int x, int y){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if(x>tmp->button_sprite.dstrect.x && x<tmp->button_sprite.dstrect.x+tmp->button_sprite.dstrect.w && y>tmp->button_sprite.dstrect.y && y<tmp->button_sprite.dstrect.y+tmp->button_sprite.dstrect.h && options==tmp->menu){
            tmp->state=2;
            switch (tmp->function){
                case play:
                    return 1;
                case skin:
                    return -2;
                case leave:
                    return -9;
                case confirm:
                    return -5;
                case left:
                    return -3;
                case right:
                    return -4;
                case locker:
                    return -6;
                default:
                    return options;
            }
        }
        tmp=tmp->next;
    }
    return options;
}

void checkOverButtons(BUTTON * buttonList, int options, int x, int y){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if(x>tmp->button_sprite.dstrect.x && x<tmp->button_sprite.dstrect.x+tmp->button_sprite.dstrect.w && y>tmp->button_sprite.dstrect.y && y<tmp->button_sprite.dstrect.y+tmp->button_sprite.dstrect.h && options==tmp->menu){
            tmp->state=3;
            break;
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