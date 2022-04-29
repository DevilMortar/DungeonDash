#include "../kernel/header.h"

int menu(BUTTON * buttonList, SKIN * skinList, SDL_Renderer *renderer, GAME *game, int *playerSkin){
    game->loop = 0;
    SDL_bool menu_active = SDL_TRUE;
    enum functions options = mainmenu; 
    SKIN *skinListTMP=skinList;
    SKIN *firstSkin=skinList;
    Uint32 frameStart;
    unsigned int frameTime;

    while (menu_active)
    {
        SDL_Event event;
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                options=checkClickButtons(buttonList, options, event.motion.x, event.motion.y);
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
            case play:
                while(firstSkin!=skinList){
                    firstSkin=firstSkin->next;
                    *playerSkin+=1;
                }
                return *playerSkin;
            case mainmenu:
                displayMainMenu(buttonList, skinList, renderer, game);
                break;
            case skin:
                displaySkinMenu(buttonList, skinListTMP, renderer, game);
                break;
            case left:
                if (skinListTMP!=NULL){
                    if (skinListTMP->previous!=NULL){
                        skinListTMP=skinListTMP->previous;
                    }
                }
                if(skinListTMP->price<0 && game->best>abs(skinListTMP->price))
                    skinListTMP->state=1;
                displaySkinMenu(buttonList, skinListTMP, renderer, game);
                options=skin;
                break;
            case right:
                if (skinListTMP!=NULL){
                    if (skinListTMP->next!=NULL){
                        skinListTMP=skinListTMP->next;
                    }
                }
                if(skinListTMP->price<0 && game->best>abs(skinListTMP->price))
                    skinListTMP->state=1;
                displaySkinMenu(buttonList, skinListTMP, renderer, game);
                options=skin;
                break;
            case confirm:
                if(skinListTMP->state==1){
                    skinList=skinListTMP;
                    options=mainmenu;
                    displayMainMenu(buttonList, skinList, renderer, game);
                }
                else{
                    options=skin;
                    displaySkinMenu(buttonList, skinListTMP, renderer, game);
                }
                break;
            case locker:
                if(skinListTMP->state==0 && skinListTMP->price > 0 && skinListTMP->price <= game->money){
                    game->money=game->money-skinListTMP->price;
                    skinListTMP->state=-1;
                }
                displaySkinMenu(buttonList, skinListTMP, renderer, game);
                options=skin;
                break;
            case leave:
                return -6;
            default:
                break;
        }
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);
        SDL_RenderPresent(renderer);
        if (game->loop == ANIMATION_LOOP)
        {
            game->loop = 0;
        }
        else
        {
            game->loop++;
        }
        if (SDL_RenderClear(renderer) != 0)
        {
            SDL_ExitWithError("SDL | Failed to clear renderer");
        }
    }
    return -1;
}

void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, GAME *game){
    SDL_RenderCopy(renderer, game->map.texture, NULL, &game->map.dstrect);
    SDL_RenderCopy(renderer, game->title.texture, NULL, &game->title.dstrect);
    //Display Record
    SDL_Color color = {255, 255, 255};
    SDL_Rect recordRect = {WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 + 150, 0, 0};
    char record[20];
    sprintf(record, "Best score   %d", game->best);
    SDL_Texture * recordTexture = renderWidgetText(record, color, 20, renderer, &recordRect);
    SDL_RenderCopy(renderer, recordTexture, NULL, &recordRect);
    // Display Buttons
    displayButtons(renderer, buttonList, mainmenu);
    skinList->skin_sprite.srcrect.x=0*skinList->skin_sprite.srcsizew;
    POSITION skinPosition = {skinList->skin_sprite.dstrect.x, skinList->skin_sprite.dstrect.y, 0};
    if (game->loop>=ANIMATION_LOOP){
        updateSprite(renderer, skinList->skin_sprite, 0, skinPosition, &skinList->skin_sprite.frame);
    }
    else {
        displaySprite(renderer, skinList->skin_sprite, 0, skinPosition, &skinList->skin_sprite.frame);
    }
}

void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, GAME *game){
    SDL_RenderCopy(renderer, game->map.texture, NULL, &game->map.dstrect);
    SDL_RenderCopy(renderer, game->titleSkin.texture, NULL, &game->titleSkin.dstrect);
    // Display Money
    SDL_Color color = {255, 255, 255};
    char money[20];
    SDL_Rect moneyRect = {WINDOW_WIDTH / 2 - (numberOfDigit(game->money) - 1) * SCORE_SIZE/2, 8, 0, 0};
    sprintf(money, "%d", game->money);
    SDL_Texture * moneyTexture = renderWidgetText(money, color, SCORE_SIZE, renderer, &moneyRect);
    SDL_RenderCopy(renderer, moneyTexture, NULL, &moneyRect);
    // Display Skin
    skinListTMP->skin_sprite.srcrect.x=0;
    POSITION skinPosition = {skinListTMP->skin_sprite.dstrect.x, skinListTMP->skin_sprite.dstrect.y, 0};
    POSITION coinPosition = {WINDOW_WIDTH / 2 - 20 - numberOfDigit(game->money) * SCORE_SIZE/2, 10, 0};
    if (game->loop>=ANIMATION_LOOP){
        updateSprite(renderer, skinListTMP->skin_sprite, 0, skinPosition, &skinListTMP->skin_sprite.frame);
        updateSprite(renderer, game->scoreCoin, 0, coinPosition, &game->scoreCoin.frame);
    }
    else {
        displaySprite(renderer, skinListTMP->skin_sprite, 0, skinPosition, &skinListTMP->skin_sprite.frame);
        displaySprite(renderer, game->scoreCoin, 0, coinPosition, &game->scoreCoin.frame);
    }
    BUTTON *tmp=buttonList;
    int wait=0;

    while(tmp!=NULL){
        if(tmp->menu==skin){
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
            if(tmp->function==left && skinListTMP->previous==NULL){
                tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
            }
            if(tmp->function==right && skinListTMP->next==NULL){
                tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
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

    // Display price if skin is locked
    if(skinListTMP->price>0 && skinListTMP->state==0){
        SDL_Rect priceRect = {WINDOW_WIDTH / 2 - 7 - (numberOfDigit(abs(skinListTMP->price)) - 1) * (SCORE_SIZE-5)/2, WINDOW_HEIGHT / 2 + 110, 0, 0};
        char price[20];
        sprintf(price, "%d", abs(skinListTMP->price));
        SDL_Texture * priceTexture = renderWidgetText(price, color, (SCORE_SIZE-5), renderer, &priceRect);
        SDL_RenderCopy(renderer, priceTexture, NULL, &priceRect);
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