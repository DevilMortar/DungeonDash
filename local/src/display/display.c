#include "../kernel/header.h"

SPRITE newSprite(SDL_Renderer *renderer, char link[255], int max, int srcsizew, int srcsizeh, int dstsize)
{
    SPRITE new;
    new.srcsizew = srcsizew;
    new.srcsizeh = srcsizeh;
    new.frame = 0;
    new.max = max;
    new.srcrect.w = srcsizew;
    new.srcrect.h = srcsizeh;
    new.texture = IMG_LoadTexture(renderer, link);
    if (new.texture == NULL)
    {
        SDL_ExitWithError("NewSprite | Failed to create texture");
    }
    else
    {
        printf("NewSprite | Sprite from %s sucessfully loaded !\n", link);
    }
    new.dstrect.w = dstsize;
    new.dstrect.h = dstsize;
    new.dstrect.x = (WINDOW_WIDTH - new.dstrect.w) / 2;
    new.dstrect.y = (WINDOW_HEIGHT - new.dstrect.h) / 2;
    return new;
}

TEXTURE newTexture(SDL_Renderer *renderer, char link[255], int w, int h)
{
    TEXTURE new;
    new.texture = NULL;

    new.texture = IMG_LoadTexture(renderer, link);
    if (new.texture == NULL)
    {
        SDL_ExitWithError("newTexture | Failed to create texture ");
    }
    else
    {
        printf("NewSprite | Texture from %s sucessfully loaded !\n", link);
    }
    new.dstrect.w = w;
    new.dstrect.h = h;
    new.dstrect.x = (WINDOW_WIDTH - new.dstrect.w) / 2;
    new.dstrect.y = (WINDOW_HEIGHT - new.dstrect.h) / 2;
    return new;
}

void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame)
{
    displaySprite(renderer, sprite, direction, position, frame);
    *frame = (*frame == sprite.max - 1) ? 0 : *frame + 1;
}

void displaySprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame)
{
    sprite.srcrect.x = *frame * sprite.srcsizew;
    switch (direction)
    {
    case 1:
        sprite.srcrect.y = sprite.srcsizeh * 3;
        break;
    case 2:
        sprite.srcrect.y = sprite.srcsizeh * 2;
        break;
    case 3:
        sprite.srcrect.y = 0;
        break;
    case 4:
        sprite.srcrect.y = sprite.srcsizeh;
        break;
    default:
        sprite.srcrect.y = 0;
        break;
    }
    sprite.dstrect.x = position.x;
    sprite.dstrect.y = position.y;
    SDL_RenderCopy(renderer, sprite.texture, &sprite.srcrect, &sprite.dstrect);
}

TEXTURE updateTexture(SDL_Renderer *renderer, TEXTURE texture, POSITION position, int width, int height)
{
    texture.dstrect.x = position.x;
    texture.dstrect.y = position.y;
    texture.dstrect.w = width;
    texture.dstrect.h = height;
    SDL_RenderCopy(renderer, texture.texture, NULL, &texture.dstrect);
    return texture;
}

void setPlayerSprite(SDL_Renderer *renderer, PLAYER *player, SKIN *skinList)
{
    SKIN *skin = skinList;
    for (int i = 0; i < player->skin-1; i++)
    {
        skin = skin->next;
    }
    player->sprite = skin->skin_sprite;
    player->sprite.dstrect.w = player->sprite.dstrect.h = SPRITE_SIZE;
}

SDL_Texture *renderWidgetText(char *message, SDL_Color color, int fontSize, SDL_Renderer *renderer, SDL_Rect *dstrect)
{
    // Open the font
    TTF_Font *font = TTF_OpenFont("asset/police/arcade.ttf", fontSize);
    if (font == NULL)
    {
        SDL_ExitWithError("SDL || TTF_OpenFont");
        return NULL;
    }

    SDL_Surface *surf = TTF_RenderText_Solid(font, message, color);
    if (surf == NULL)
    {
        TTF_CloseFont(font);
        SDL_ExitWithError("SDL || TTF_RenderText");
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == NULL)
    {
        SDL_ExitWithError("SDL || CreateTexture");
    }
    // Take the size
    dstrect->w = surf->w;
    dstrect->h = surf->h;
    // Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

int displayGame(SDL_Renderer *renderer, PLAYER *player, TEXTURE map, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], TEXTURE hole, GAME *game)
{
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            if (Hole[y][x] == 1)
            {
                POSITION temp;
                temp.x = CASE_OFFSET_X + x * CASE_SIZE;
                temp.y = CASE_OFFSET_Y + y * CASE_SIZE;
                hole = updateTexture(renderer, hole, temp, SPRITE_SIZE, SPRITE_SIZE);
            }
        }
    }
    if (game->loop % ANIMATION_LOOP == 0)
    {
        if (game->status == 0)
        {
            POSITION scorecoin;
            scorecoin.x = WINDOW_WIDTH/2 - SCORE_SIZE;
            scorecoin.y = 10;
            updateSprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
            updateSprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
            updateSprite(renderer, game->scoreCoin, 0, scorecoin, &game->scoreCoin.frame);
        }
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning > 0)
                {
                    updateSprite(renderer, fireball.warning, 0, temp->position, &temp->frame);
                }
                else
                {
                    updateSprite(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame);
                }
                temp = temp->next;
            }
        }
    }
    else
    {
        if (game->status == 0)
        {
            POSITION scorecoin;
            scorecoin.x = WINDOW_WIDTH/2 - SCORE_SIZE;
            scorecoin.y = 10;
            displaySprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
            displaySprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
            displaySprite(renderer, game->scoreCoin, 0, scorecoin, &game->scoreCoin.frame);
        }
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning > 0)
                {
                    displaySprite(renderer, fireball.warning, 0, temp->position, &temp->frame);
                }
                else
                {
                    displaySprite(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame);
                }
                temp = temp->next;
            }
        }
    }
    if (game->status == 0) {
        SDL_Color color_white = {255,255,255};
        char scorestr[WIDGET_LENGTH];
        sprintf(scorestr, "%d", game->score);
        SDL_Texture *scoreCoin = renderWidgetText(scorestr, color_white, SCORE_SIZE, renderer, &game->coinrect);
        SDL_RenderCopy(renderer, scoreCoin, NULL, &game->coinrect);
    }
    if (game->status == 1)
    {
        SDL_Color color_white = {255,255,255};
        char scorestr[WIDGET_LENGTH];
        sprintf(scorestr, "Score     %d", game->score);
        SDL_Texture *scoreprint = renderWidgetText(scorestr, color_white, END_SCORE_SIZE, renderer, &game->endscorerect);
        sprintf(scorestr, "Record   %d", game->best);
        SDL_Texture *recordprint = renderWidgetText(scorestr, color_white, END_SCORE_SIZE, renderer, &game->endbestrect);
        if (game->deathAnimation.frame < game->deathAnimation.max - 1)
        {
            updateSprite(renderer, game->deathAnimation, 0, player->position, &game->deathAnimation.frame);
        }
        else
        {
            displaySprite(renderer, game->deathAnimation, 0, player->position, &game->deathAnimation.frame);
            game->endscreen.dstrect.x = WINDOW_WIDTH/2 - game->endscreen.dstrect.w/2;
            game->endscreen.dstrect.y = WINDOW_HEIGHT/2 - game->endscreen.dstrect.h/2;  
            SDL_RenderCopy(renderer, game->endscreen.texture, NULL, &game->endscreen.dstrect);
            SDL_RenderCopy(renderer, scoreprint, NULL, &game->endscorerect);
            SDL_RenderCopy(renderer, recordprint, NULL, &game->endbestrect);
        }
    }
    return 0;
}

void SDL_LimitFPS(unsigned int limit)
{
    float delay = FPS_DELAY;
    if (delay > limit)
    {
        SDL_Delay(delay - limit);
    }
    else
    {
        printf("WARNING | Low FPS ! Time : %dms\n", limit);
    }
}

void SDL_initGameView(SDL_Window ** window, SDL_Renderer ** renderer) {
    if (!IMG_Init(IMG_INIT_PNG))
        printf("\033[1;31mIMG INIT: %s\033[0m\n", IMG_GetError());
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_ExitWithError("SDL | Failed to initialize");
    }
    if (TTF_Init() != 0)
    {
        SDL_ExitWithError("SDL | TTF: Failed to initialize");
    }

    *window = SDL_CreateWindow("Dungeon Dash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (*window == NULL)
    {
        SDL_ExitWithError("SDL | Failed to create a window");
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);

    if (*renderer == NULL)
    {
        SDL_ExitWithError("SDL | Failed to create a renderer");
    }

    SDL_SetRenderDrawColor(*renderer, 21, 33, 44, 255);

    printf("SDL | Initialized with success !\n");
}