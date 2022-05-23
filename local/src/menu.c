#include "../include/menu.h"

int startMenu(BUTTON *buttonList, LIST_SKIN *skinList, PLAYER *player, SDL_Renderer *renderer, GAME *game)
{
    game->loop = 0;
    SDL_bool menu_active = SDL_TRUE;
    enum functions options = none;
    game->menu = mainMenu;
    SKIN * skinOnDisplay = resetSkinSize(&player->skin);
    Uint32 frameStart;
    unsigned int frameTime;
    printf("\nGame status | Menu Launched\n");

    while (menu_active)
    {
        SDL_Event event;
        frameStart = SDL_GetTicks();
        SDL_RenderCopy(renderer, game->background.texture, NULL, &game->background.dstrect);
        SDL_RenderCopy(renderer, game->map.texture, NULL, &game->map.dstrect);
        SDL_RenderCopy(renderer, game->title.texture, NULL, &game->title.dstrect);
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

        switch (options)
        {
        case play:
            setPlayerSprite(player, skinOnDisplay);
            SL_playSong("play", 50);
            return 2;
        case left:
            if (skinOnDisplay->previous == NULL)
            {
                skinOnDisplay = skinList->last;
            }
            else
            {
                skinOnDisplay = skinOnDisplay->previous;
            }
            SL_playSong("left", 50);
            break;
        case right:
            if (skinOnDisplay->next == NULL)
            {
                skinOnDisplay = skinList->first;
            }
            else
            {
                skinOnDisplay = skinOnDisplay->next;
            }
            SL_playSong("right", 50);
            break;
        case confirm:
            if (skinOnDisplay->state == -1)
            {
                skinOnDisplay->state = 1;
                game->menu = mainMenu;
            }
            SL_playSong("drum", 50);
            break;
        case locker:
            if (skinOnDisplay->state == 0)
            {
                if (skinOnDisplay->price > 0 && skinOnDisplay->price <= game->money)
                {
                    game->money = game->money - skinOnDisplay->price;
                    skinOnDisplay->state = -1;
                }
            }
            break;
        case reset:
            resetData(skinList, game);
            skinOnDisplay = skinList->first;
            SL_playSong("reset", 50);
            break;
        case leave:
            return -6;
            SL_playSong("back", 50);
        case backToMenu:
            game->menu = mainMenu;
            break;
        case skin:
            game->menu = skinMenu;
            SL_playSong("next", 50);
            break;
        case sound:
            if (SL_isPlaying())
            {
                SL_playSong("next", 50);
                SL_mute();
            }
            else
            {
                SL_unmute();
                SL_playSong("next", 50);
            }
            break;
        case none:
            break;
        default:
            break;
        }

        options = none;

        switch (game->menu)
        {
        case mainMenu:
            displayMainMenu(buttonList, skinOnDisplay, renderer, game);
            break;
        case skinMenu:
            displaySkinMenu(buttonList, skinOnDisplay, renderer, game);
            break;
        default:
            break;
        }

        SDL_RenderCopy(renderer, buttonList->button_sprite.texture, &buttonList->button_sprite.srcrect, &buttonList->button_sprite.dstrect);
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

void displayMainMenu(BUTTON *buttonList, SKIN *skinList, SDL_Renderer *renderer, GAME *game)
{
    // Display Record
    SDL_Rect recordRect = {WINDOW_WIDTH / 2 - 130, WINDOW_HEIGHT / 2 + 150, 0, 0};
    displayTextAndNumber(renderer, "Best Score   ", game->best, NULL, 20, &recordRect);
    // Display Buttons
    BUTTON *tmp = buttonList;
    while (tmp != NULL)
    {
        if (tmp->function == sound)
        {
            if (SL_isPlaying() == false)
            {
                if (tmp->state == 0)
                {
                    tmp->button_sprite.srcrect.x = tmp->button_sprite.srcsizew * 3;
                }
                else if (tmp->state == 1)
                {
                    tmp->button_sprite.srcrect.x = tmp->button_sprite.srcsizew * 4;
                }
            }
        }
        tmp = tmp->next;
    }
    displayButtonList(renderer, buttonList, mainMenu);
    POSITION skinPosition = {skinList->skin_sprite.dstrect.x, skinList->skin_sprite.dstrect.y, 0};
    // Display Skin
    updateSpriteIfNeeded(renderer, skinList->skin_sprite, 0, skinPosition, &skinList->skin_sprite.frame, game->loop);
}

void displaySkinMenu(BUTTON *buttonList, SKIN *skinOnDisplay, SDL_Renderer *renderer, GAME *game)
{
    // Display Money
    SDL_Rect moneyRect = {WINDOW_WIDTH / 2 + 5, 10, 0, 0};
    displayNumber(renderer, game->money, NULL, MONEY_SIZE, &moneyRect);
    POSITION coinPosition = {WINDOW_WIDTH / 2 - 15 - numberOfDigit(game->money) * (MONEY_SIZE) / 2, 8, 0};
    updateSpriteIfNeeded(renderer, game->scoreCoin, 0, coinPosition, &game->scoreCoin.frame, game->loop);
    // Display Skin
    skinOnDisplay->skin_sprite.srcrect.x = 0;
    POSITION skinPosition = {skinOnDisplay->skin_sprite.dstrect.x, skinOnDisplay->skin_sprite.dstrect.y, 0};
    // Display Skin
    updateSpriteIfNeeded(renderer, skinOnDisplay->skin_sprite, 0, skinPosition, &skinOnDisplay->skin_sprite.frame, game->loop);
    // Display Buttons
    BUTTON *tmp = buttonList;
    while (tmp != NULL)
    {
        if (tmp->menu == skin)
        {
            tmp->button_sprite.srcrect.x = tmp->button_sprite.srcsizew * tmp->state;
            if (tmp->function == locker)
            {
                if (skinOnDisplay->state != 0)
                {
                    tmp->enabled = false;
                }
                else
                {
                    tmp->enabled = true;
                }
            }
        }
        if (tmp->function == confirm && skinOnDisplay->state == 0)
        {
            tmp->button_sprite.srcrect.x = tmp->button_sprite.srcsizew * 2;
        }
        tmp = tmp->next;
    }
    displayButtonList(renderer, buttonList, skinMenu);

    // Display price if skin is locked
    if (skinOnDisplay->state != 1 && skinOnDisplay->state != -1)
    {
        if (skinOnDisplay->price <= 0)
        {
            if (game->best >= abs(skinOnDisplay->price))
            {
                skinOnDisplay->state = -1;
            }
            else
            {
                SDL_Rect unlockCondition = {WINDOW_WIDTH / 2 - 274, WINDOW_HEIGHT / 2 - 125, 0, 0};
                displayText(renderer, "Unlock this skin with your highscore", NULL, 17, &unlockCondition);
            }
        }
        SDL_Rect priceRect = {WINDOW_WIDTH / 2 - 7, WINDOW_HEIGHT / 2 + 110, 0, 0};
        displayNumber(renderer, abs(skinOnDisplay->price), NULL, MONEY_SIZE, &priceRect);
    }
    else if (skinOnDisplay->state == 1)
    {
        skinOnDisplay->state = -1;
    }
}

LIST_SKIN *createSkin(SDL_Renderer *renderer, char *link, LIST_SKIN *skinList, int state, int price, int srcsizew, int srcsizeh)
{
    SKIN *new;
    new = malloc(sizeof(SKIN));
    new->skin_sprite = newSprite(renderer, link, 3, srcsizew, srcsizeh, SKIN_W_H, SKIN_W_H);
    new->skin_sprite.dstrect.x = CASE_OFFSET_X + 2 * CASE_SIZE - 20;
    new->skin_sprite.dstrect.y = CASE_OFFSET_Y + 2 * CASE_SIZE - 20;
    new->state = state;
    new->price = price;
    new->previous = NULL;
    new->next = NULL;
    return addSkinInList(skinList, new);
}

LIST_SKIN *addSkinInList(LIST_SKIN *skinList, SKIN *newSkin)
{
    if (skinList->first != NULL)
    {
        newSkin->next = skinList->first;
        skinList->first->previous = newSkin;
        skinList->first = newSkin;
    }
    else
    {
        skinList->first = newSkin;
        skinList->last = newSkin;
    }
    return skinList;
}

SKIN *browseSkin(SKIN *tmp, int direction, int best)
{
    if (tmp != NULL)
    {
        if (direction < 0)
        {
            if (tmp->previous != NULL)
            {
                tmp = tmp->previous;
            }
        }
        else
        {
            if (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
        }
    }
    if (tmp->price < 0 && best >= abs(tmp->price))
        tmp->state = 1;
    return tmp;
}

void freeSkinList(LIST_SKIN *skinList)
{
    SKIN *tmp = skinList->first;
    while (tmp != NULL)
    {
        SKIN *tmp2 = tmp->next;
        free(tmp);
        tmp = tmp->next;
    }
}
