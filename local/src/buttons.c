#include "../include/button.h"

/*

Button state :
0 = normal
1 = hover
2 = click

*/

BUTTON * createButton(SDL_Renderer *renderer, char *link, BUTTON * buttonList, int w, int h, int x, int y, functions function, menu menu, int srcsizew, int srcsizeh, int numberOfSprite, bool enabled){
    BUTTON *new = malloc(sizeof(BUTTON));
    new->button_sprite=newSprite(renderer, link, 0, srcsizew, srcsizeh, w, h);
    new->button_sprite.dstrect.x=x;
    new->button_sprite.dstrect.y=y;
    new->state=normal;
    new->function=function;
    new->menu=menu;
    new->enabled=true;
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
        if (tmp->menu==menu && tmp->enabled){
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
        if (tmp->menu==menu && tmp->enabled){
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
        if (tmp->menu == menu && tmp->enabled){
            SDL_RenderCopy(renderer, tmp->button_sprite.texture, &tmp->button_sprite.srcrect, &tmp->button_sprite.dstrect);
        }
        tmp=tmp->next;
    }
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