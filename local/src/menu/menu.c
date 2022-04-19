#include "../kernel/header.h"

int menu(BUTTON * buttonList, SKIN * skinList, SDL_Renderer *renderer, TEXTURE map, TEXTURE title){
    SDL_bool menu_active = SDL_TRUE;
    int options=-1;
    int skinChoice=1;
    SKIN *skinListTMP=skinList;
    SKIN *firstSkin=skinList;

    while (menu_active)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                options=checkClickButtons(buttonList, skinListTMP, options, event.motion.x, event.motion.y);
                break;
            case SDL_QUIT:
                menu_active = SDL_FALSE;
                break;
            default:
                resetButtonState(buttonList);
                checkOverButtons(buttonList, options, event.motion.x, event.motion.y);
                break;
            }
        }
        switch(options){
            case 1:
                while(firstSkin!=skinList){
                    firstSkin=firstSkin->next;
                    skinChoice+=1;
                }
                return skinChoice;
                return 1;
            case -1:
                displayMainMenu(buttonList, skinList, renderer, map, title);
                break;
            case -2:
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title);
                break;
            case -3:
                skinListTMP=skinListTMP->previous;
                if(skinListTMP->price<0 && 1>skinListTMP->price);
                    skinListTMP->state=1;
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title);
                options=-2;
                break;
            case -4:
                skinListTMP=skinListTMP->next;
                if(skinListTMP->price<0 && 1>floor(skinListTMP->price))
                    skinListTMP->state=1;
                displaySkinMenu(buttonList, skinListTMP, renderer, map, title);
                options=-2;
                break;
            case -5:
                if(skinListTMP->state==1){
                    skinList=skinListTMP;
                    options=-1;
                    displayMainMenu(buttonList, skinList, renderer, map, title);
                }
                else{
                    options=-2;
                    displaySkinMenu(buttonList, skinListTMP, renderer, map, title);
                }
                break;
            case -6:
                if(skinListTMP->state==0 && skinListTMP->price > 0 && skinListTMP->price <= 1){
                    skinListTMP->state=-1;
                }
                options=-2;
                break;
            case -9:
                return -6;
            default:
                break;
        }
    }
    return -1;
}

void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, TEXTURE map, TEXTURE title){
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);
    SDL_RenderCopy(renderer, title.texture, NULL, &title.dstrect);
    BUTTON *tmp=buttonList;

    while(tmp!=NULL){
        if(tmp->menu==-1){
            switch(tmp->state){
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
    skinList->skin_sprite.srcrect.x=0*skinList->skin_sprite.srcsizew;
    SDL_RenderCopy(renderer, skinList->skin_sprite.texture, &skinList->skin_sprite.srcrect, &skinList->skin_sprite.dstrect);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, TEXTURE map, TEXTURE title){
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);
    SDL_RenderCopy(renderer, title.texture, NULL, &title.dstrect);
    skinListTMP->skin_sprite.srcrect.x=0;
    SDL_RenderCopy(renderer, skinListTMP->skin_sprite.texture, &skinListTMP->skin_sprite.srcrect, &skinListTMP->skin_sprite.dstrect);
    BUTTON *tmp=buttonList;
    int wait=0;

    while(tmp!=NULL){
        if(tmp->menu==-2){
            switch(tmp->state){
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
            if(tmp->function==locker && skinListTMP->state==1){
                 tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*5;
            }
            if(tmp->function==locker && skinListTMP->state<0){
                 tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*(-skinListTMP->state+1);
                 skinListTMP->state=skinListTMP->state-1;
                 if(skinListTMP->state==-4){skinListTMP->state=1;}
                 wait=1;
            }
            if(tmp->function==confirm && skinListTMP->state==0){
                tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
            }
            SDL_RenderCopy(renderer, tmp->button_sprite.texture, &tmp->button_sprite.srcrect, &tmp->button_sprite.dstrect);
        }
        tmp=tmp->next;
    }

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    if(wait==1){
        SDL_Delay(200);
        wait=0;
    }
}

SKIN * createSkin(SDL_Renderer *renderer, char link[255], SKIN * skinList, int w, int h, int x, int y, int state, int price, int srcsizew, int srcsizeh){
    SKIN *new;
    new=malloc(sizeof(SKIN));
    new->skin_sprite=newSprite(renderer, link, 3, srcsizew, srcsizeh, 0);
    new->skin_sprite.dstrect.w=w;
    new->skin_sprite.dstrect.h=h;
    new->skin_sprite.dstrect.x=x;
    new->skin_sprite.dstrect.y=y;
    new->state=state;
    new->price=price;
    new->previous=NULL;
    new->next=NULL;
    return addSkinInList(skinList, new);
}

SKIN * addSkinInList(SKIN *skinList, SKIN *newSkin){
    if(skinList != NULL && newSkin !=NULL){
        newSkin->next=skinList;
        skinList->previous=newSkin;
        skinList=newSkin;
    }
    else{
        skinList=newSkin;
    }
    return skinList;
}

void closeSkinList(SKIN *skinList){
    SKIN *tmp=skinList;
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }
    tmp->next=skinList;
    skinList->previous=tmp;
}