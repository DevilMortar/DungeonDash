#include "display.h"

SPRITE newSprite(SDL_Renderer *renderer, char lien[255], int frame, int srcsize, int dstsize)
{
    SPRITE new;
    new.srcsize = srcsize;
    new.frame = 0;
    new.max = frame;
    new.srcrect.w = srcsize;
    new.srcrect.h = srcsize;
    new.texture = IMG_LoadTexture(renderer, lien);
    if (new.texture == NULL)
    {
        SDL_ExitWithError("NewSprite | Failed to create texture");
    }
    else {
        printf("NewSprite | Sprite from %s sucessfully loaded !\n",lien);
    }
    new.dstrect.w = dstsize;
    new.dstrect.h = dstsize;
    new.dstrect.x = (WINDOW_WIDTH - new.dstrect.w) / 2;
    new.dstrect.y = (WINDOW_HEIGHT - new.dstrect.h) / 2;
    return new;
}

TEXTURE newTexture(SDL_Renderer *renderer, char lien[255])
{
    TEXTURE new;
    new.texture = NULL;

    new.texture = IMG_LoadTexture(renderer, lien);
    if (new.texture == NULL)
    {
        SDL_ExitWithError("newTexture | Failed to create texture");
    }
    else {
        printf("NewSprite | Texture from %s sucessfully loaded !\n",lien);
    }
    new.dstrect.w = 600;
    new.dstrect.h = 600;
    new.dstrect.x = (WINDOW_WIDTH - new.dstrect.w) / 2;
    new.dstrect.y = (WINDOW_HEIGHT - new.dstrect.h) / 2;
    return new;
}

void updateSprite(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame)
{
    displaySprite(renderer,sprite, direction, position, frame);
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

int displayGame(SDL_Renderer *renderer, PLAYER *player, TEXTURE map, LIST_OBSTACLE fireball, LIST_OBSTACLE laser, COIN *coin, int Hole[7][7], TEXTURE hole, int loop)
{
    SDL_RenderCopy(renderer, map.texture, NULL, &map.dstrect);

    if (loop % ANIMATION_LOOP == 0)
    {
        updateSprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
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
        updateSprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning > 0) {
                    updateSprite(renderer, fireball.warning, 0, temp->position, &temp->frame);
                }
                else {
                    updateSprite(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame);
                }
                temp = temp->next;
            }
        }
    }
    else {
        displaySprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
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
        displaySprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning > 0) {
                    displaySprite(renderer, fireball.warning, 0, temp->position, &temp->frame);
                }
                else {
                    displaySprite(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame);
                }
                temp = temp->next;
            }
        }
    }
}

void SDL_LimitFPS(unsigned int limit)
{
    unsigned int ticks = SDL_GetTicks();

    if (limit < ticks)
        return;
    else if (limit > ticks + FPS_DELAY)
        SDL_Delay(FPS_DELAY);
    else
        SDL_Delay(limit - ticks);
}