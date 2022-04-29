#include "../kernel/header.h"

int startMenu(BUTTON * buttonList, SKIN * skinList, SKIN * firstSkin, SDL_Renderer *renderer, GAME *game, int *playerSkin){
    game->loop = 0;
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
                options=checkClickButtons(buttonList, options, game->menu, event.motion.x, event.motion.y);
                break;
                
            case SDL_QUIT:
                menu_active = SDL_FALSE;
                break;
            default:
                break;
            }
        }
        resetButtonState(buttonList);
        checkOverButtons(buttonList, options, game->menu, event.motion.x, event.motion.y);
        switch(options){
            case play:
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
    SDL_Rect recordRect = {WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 + 150, 0, 0};
    char record[20];
    sprintf(record, "Best score   %d", game->best);
    SDL_Texture * recordTexture = renderWidgetText(record, NULL, 20, renderer, &recordRect);
    SDL_RenderCopy(renderer, recordTexture, NULL, &recordRect);
    // Display Buttons
    displayButtons(renderer, buttonList, mainMenu);
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
    SDL_Rect moneyRect = {WINDOW_WIDTH / 2, 10, 0, 0};
    displayNumber(renderer, game->money, NULL, SCORE_SIZE-5, &moneyRect);
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
        SDL_Rect priceRect = {WINDOW_WIDTH / 2 - 7, WINDOW_HEIGHT / 2 + 110, 0, 0};
        displayNumber(renderer, skinListTMP->price, NULL, SCORE_SIZE-5, &priceRect);
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

