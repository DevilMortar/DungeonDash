#include "../include/button.h"

/*

Button state :
0 = normal
1 = hover
2 = click

*/

BUTTON * createButton(SDL_Renderer *renderer, char *link, BUTTON * buttonList, int w, int h, int x, int y, functions function, menu menu, int srcsizew, int srcsizeh){
    BUTTON *new = malloc(sizeof(BUTTON));
    new->button_sprite=newSprite(renderer, link, 3, srcsizew, srcsizeh, 0);
    SDL_Rect dstrect = {x, y, w, h};
    new->button_sprite.dstrect=dstrect;
    new->state=normal;
    new->function=function;
    new->menu=menu;
    new->next=NULL;
    return addButtonInList(buttonList, new);
}

BUTTON *addButtonInList(BUTTON *buttonList, BUTTON * newButton){
    if(buttonList != NULL && newButton != NULL){
        newButton->next = buttonList;
        buttonList = newButton;
    } else{
        buttonList = newButton;
    }
    return buttonList;
}

BUTTON * checkClickButtons(BUTTON * buttonList, enum functions *options, int menu, int x, int y){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if (tmp->menu==menu){
            SDL_Point mouse = {x, y};
            if(SDL_PointInRect(&mouse, &tmp->button_sprite.dstrect)){
                *options = tmp->function;
                buttonChangeState(tmp, 2);
                return tmp;
            }
        }
        tmp=tmp->next;
    }
}

void checkOverButtons(BUTTON * buttonList, int options, int menu, int x, int y){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if (tmp->menu==menu){
            SDL_Point mouse = {x, y};
            if(SDL_PointInRect(&mouse, &tmp->button_sprite.dstrect)){
                buttonChangeState(tmp, 1);
            }
        }
        tmp=tmp->next;
    }
}

void resetButtonState(BUTTON * buttonList){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        buttonChangeState(tmp, 0);
        tmp=tmp->next;
    }
}

void displayButtonList(SDL_Renderer *renderer, BUTTON * buttonList, int menu){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        if (tmp->menu == menu){
            displayButton(renderer, tmp);
        }
        tmp=tmp->next;
    }
}

void displayButton(SDL_Renderer *renderer, BUTTON * button){
    SDL_RenderCopy(renderer, button->button_sprite.texture, &button->button_sprite.srcrect, &button->button_sprite.dstrect);
}

void buttonChangeState(BUTTON * button, int state){
    if (button != NULL){
        button->state=state;
        button->button_sprite.srcrect.x=button->state*button->button_sprite.srcrect.w;
    }
}

void freeButtons(BUTTON * buttonList){
    BUTTON * tmp=buttonList;
    while(tmp!=NULL){
        tmp=tmp->next;
        free(buttonList);
        buttonList=tmp;
    }
}