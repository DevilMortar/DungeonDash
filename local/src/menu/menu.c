#include "../kernel/header.h"

int startMenu(BUTTON * buttonList, SKIN * skinList, SKIN * firstSkin, SDL_Renderer *renderer, GAME *game, int *playerSkin){
    game->loop = 0;
    for (int i=1; i<*playerSkin; i++){
        skinList = skinList->next;
    }
    SDL_bool menu_active = SDL_TRUE;
    enum functions options = none;
    game->menu = mainMenu; 
    SKIN *skinListTMP=skinList;
    Uint32 frameStart;
    unsigned int frameTime;

    while (menu_active)
    {
        SDL_Event event;
        frameStart = SDL_GetTicks();
        SDL_RenderCopy(renderer, game->map.texture, NULL, &game->map.dstrect);
        SDL_RenderCopy(renderer, game->titleSkin.texture, NULL, &game->titleSkin.dstrect);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            options = leave;
                            continue;
                        default:
                            continue;
                    }
                    break;
            case SDL_MOUSEBUTTONDOWN:
                checkClickButtons(buttonList, &options, game->menu, event.motion.x, event.motion.y);
                break;
                
            case SDL_QUIT:
                menu_active = SDL_FALSE;
                break;
            default:
                resetButtonState(buttonList);
                checkOverButtons(buttonList, options, game->menu, event.motion.x, event.motion.y);
                break;
            }
        }
        switch(options){
            case play:
                *playerSkin = 1;
                while(firstSkin!=skinList){
                    firstSkin=firstSkin->next;
                    *playerSkin+=1;
                }
                return *playerSkin;
            case left:
                skinListTMP = browseSkin(skinListTMP, -1, game->best);
                break;
            case right:
                skinListTMP = browseSkin(skinListTMP, 1, game->best);
                break;
            case confirm:
                if(skinListTMP->state==1){
                    skinList=skinListTMP;
                    game->menu=mainMenu;
                }
                break;
            case locker:
                if(skinListTMP->state==0 && skinListTMP->price > 0 && skinListTMP->price <= game->money){
                    game->money=game->money-skinListTMP->price;
                    skinListTMP->state=-1;
                }
                break;
            case reset:
                resetData(firstSkin, game);
                break;
            case leave:
                return -6;
            case backToMenu:
                game->menu=mainMenu;
                break;
            case skin:
                game->menu=skinMenu;
                break;
            case none:
                break;
            default:
                break;
        }
        options=none;
        switch (game->menu)
        {
            case mainMenu:
                displayMainMenu(buttonList, skinListTMP, renderer, game);
                break;
            case skinMenu:
                displaySkinMenu(buttonList, skinListTMP, renderer, game);
                break;
            default:
                break;
        }
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);
        if (game->loop >= 10000)
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
    //Display Record
    SDL_Rect recordRect = {WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 + 150, 0, 0};
    displayTextAndNumber(renderer, "Best Score   ", game->best, NULL, 20, &recordRect);
    // Display Buttons
    displayButtonList(renderer, buttonList, mainMenu);
    POSITION skinPosition = {skinList->skin_sprite.dstrect.x, skinList->skin_sprite.dstrect.y, 0};
    // Display Skin
    updateSpriteIfNeeded(renderer, skinList->skin_sprite, 0, skinPosition, &skinList->skin_sprite.frame, game->loop);
}

void displaySkinMenu(BUTTON *buttonList, SKIN *skinListTMP, SDL_Renderer *renderer, GAME *game){
    // Display Money
    SDL_Rect moneyRect = {WINDOW_WIDTH / 2 + 5, 10, 0, 0};
    displayNumber(renderer, game->money, NULL, MONEY_SIZE, &moneyRect);
    POSITION coinPosition = {WINDOW_WIDTH / 2 - 15 - numberOfDigit(game->money) * (MONEY_SIZE)/2, 8, 0};
    updateSpriteIfNeeded(renderer, game->scoreCoin, 0, coinPosition, &game->scoreCoin.frame, game->loop);
    // Display Skin
    skinListTMP->skin_sprite.srcrect.x=0;
    POSITION skinPosition = {skinListTMP->skin_sprite.dstrect.x, skinListTMP->skin_sprite.dstrect.y, 0};
    // Display Skin
    updateSpriteIfNeeded(renderer, skinListTMP->skin_sprite, 0, skinPosition, &skinListTMP->skin_sprite.frame, game->loop);

    // PATCH THIS FK SHIT
    BUTTON *tmp=buttonList;

    while(tmp!=NULL){
        if(tmp->menu==skin){
            tmp->button_sprite.srcrect.x= tmp->button_sprite.srcsizew*tmp->state;
            if(tmp->function==left){
                if (skinListTMP->previous==NULL)
                    buttonChangeState(tmp, 3);
            }
            else if(tmp->function==right){
                if (skinListTMP->next==NULL)
                    buttonChangeState(tmp, 3);
            }
            if(tmp->function==locker && skinListTMP->state==1){
                 tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*5;
            }
            if(tmp->function==locker && skinListTMP->state<0){
                 tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*(-skinListTMP->state+1);
                 skinListTMP->state=skinListTMP->state-1;
                 if(skinListTMP->state==-4){skinListTMP->state=1;}
            }
            if(tmp->function==confirm && skinListTMP->state==0){
                tmp->button_sprite.srcrect.x=tmp->button_sprite.srcsizew*2;
            }
            SDL_RenderCopy(renderer, tmp->button_sprite.texture, &tmp->button_sprite.srcrect, &tmp->button_sprite.dstrect);
        }
        tmp=tmp->next;
    }

    // Display price if skin is locked
    if(skinListTMP->state==0){
        if (skinListTMP->price<0) {
            SDL_Rect unlockCondition = {WINDOW_WIDTH / 2 - 274, WINDOW_HEIGHT / 2 - 125, 0, 0};
            displayText(renderer, "Unlock this skin with your highscore", NULL, 17, &unlockCondition);
        }
        SDL_Rect priceRect = {WINDOW_WIDTH / 2 - 7, WINDOW_HEIGHT / 2 + 110, 0, 0};
        displayNumber(renderer, abs(skinListTMP->price), NULL, MONEY_SIZE, &priceRect);
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

SKIN * browseSkin(SKIN * tmp, int direction, int best){
    if (tmp!=NULL){
        if(direction<0){
            if (tmp->previous!=NULL){
                tmp=tmp->previous;
            }
        }
        else{
            if (tmp->next!=NULL){
                tmp=tmp->next;
            }
        }
    }
    if(tmp->price<0 && best>=abs(tmp->price))
        tmp->state=1;
    return tmp;
}

void freeSkinList(SKIN *skinList){
    SKIN *tmp;
    while(skinList!=NULL){
        tmp=skinList;
        skinList=skinList->next;
        free(tmp);
    }
}

