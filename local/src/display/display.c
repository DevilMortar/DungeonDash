#include "display.h"

SPRITE newSprite(SDL_Renderer *renderer, char lien[255], int max, int srcsize, int dstsize)
{
    SPRITE new;
    new.srcsize = srcsize;
    new.frame = 0;
    new.max = max;
    new.srcrect.w = srcsize;
    new.srcrect.h = srcsize;
    new.texture = IMG_LoadTexture(renderer, lien);
    if (new.texture == NULL)
    {
        SDL_ExitWithError("NewSprite | Failed to create texture");
    }
    else
    {
        printf("NewSprite | Sprite from %s sucessfully loaded !\n", lien);
    }
    new.dstrect.w = dstsize;
    new.dstrect.h = dstsize;
    new.dstrect.x = (WINDOW_WIDTH - new.dstrect.w) / 2;
    new.dstrect.y = (WINDOW_HEIGHT - new.dstrect.h) / 2;
    return new;
}

TEXTURE newTexture(SDL_Renderer *renderer, char lien[255], int w, int h)
{
    TEXTURE new;
    new.texture = NULL;

    new.texture = IMG_LoadTexture(renderer, lien);
    if (new.texture == NULL)
    {
        SDL_ExitWithError("newTexture | Failed to create texture");
    }
    else
    {
        printf("NewSprite | Texture from %s sucessfully loaded !\n", lien);
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
    sprite.srcrect.x = *frame * sprite.srcsize;
    switch (direction)
    {
    case 1:
        sprite.srcrect.y = sprite.srcsize * 3;
        break;
    case 2:
        sprite.srcrect.y = sprite.srcsize * 2;
        break;
    case 3:
        sprite.srcrect.y = 0;
        break;
    case 4:
        sprite.srcrect.y = sprite.srcsize;
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

void setPlayerSprite(SDL_Renderer *renderer, PLAYER *player)
{
    char lien[255] = "asset/texture/player/";
    char skin[3];
    sprintf(skin, "%d", player->skin);
    strcat(lien, skin);
    strcat(lien, ".png");
    player->sprite = newSprite(renderer, lien, 3, 32, SPRITE_SIZE);
}

SDL_Texture *renderWidgetText(char *message, SDL_Color color, int fontSize, SDL_Renderer *renderer, SDL_Rect * dstrect)
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

int displayGame(SDL_Renderer *renderer, PLAYER *player, TEXTURE map, LIST_OBSTACLE fireball, COIN *coin, int Hole[7][7], TEXTURE hole, int loop, END *end, int score)
{
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);

    if (loop % ANIMATION_LOOP == 0)
    {

        for (int y = 0; y < 7; y++)
        {
            for (int x = 0; x < 7; x++)
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
        if (end->status == 0)
        {
            updateSprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
            updateSprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
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
        for (int y = 0; y < 7; y++)
        {
            for (int x = 0; x < 7; x++)
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
        if (end->status == 0)
        {
            displaySprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
            displaySprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
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

    if (end->status == 1)
    {
        /*
        SDL_Texture *text, *option, *scoreprint;
        char name[WIDGET_LENGTH], start[WIDGET_LENGTH], scorestr[WIDGET_LENGTH];
        sprintf(scorestr, "Score : %d", score);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &end->rect3);

        memset(name, 0x00, WIDGET_LENGTH);
        memset(start, 0x00, WIDGET_LENGTH);
        strcpy(name, "GAME OVER");
        strcpy(start, "Restart");
        color_red.r = 255;
        color_red.b = color_red.g = 0;
        color_white.r = color_white.g = color_white.b = 255;
        text = renderWidgetText(name, color_red, 72, renderer);
        option = renderWidgetText(start, color_red, 40, renderer);
        scoreprint = renderWidgetText(scorestr, color_white, 40, renderer);
        SDL_RenderCopy(renderer, text, NULL, &end->rect);
        SDL_RenderCopy(renderer, option, NULL, &end->rect2);
        */

        SDL_Color color_red;
        SDL_Color color_white;
        color_white.r = color_white.g = color_white.b = 255;
        char scorestr[WIDGET_LENGTH];
        sprintf(scorestr, "Score   %d", score);
        SDL_Texture * scoreprint = renderWidgetText(scorestr, color_white, END_SCORE_SIZE, renderer, &end->scorerect);

        if (end->skull.frame < end->skull.max - 1)
        {
            updateSprite(renderer, end->skull, 0, player->position, &end->skull.frame);
        }
        else
        {
            displaySprite(renderer, end->skull, 0, player->position, &end->skull.frame);
            SDL_RenderCopy(renderer, end->texture.texture, NULL, &end->texture.dstrect);
            SDL_RenderCopy(renderer, scoreprint, NULL, &end->scorerect);
        }
    }
    return 0;
}

void SDL_LimitFPS(unsigned int limit)
{
    if (FPS_DELAY > limit)
    {
        SDL_Delay(FPS_DELAY - limit);
    }
}