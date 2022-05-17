#include "../include/display.h"

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
    if (new.texture == NULL) {
        SDL_ExitWithError("NewSprite | Failed to create texture");
    } else {
        printf("NewSprite | Sprite from %s successfully loaded !\n", link);
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
        SDL_ExitWithError("NewTexture | Failed to create texture ");
    }
    else
    {
        printf("NewTexture | Texture from %s successfully loaded !\n", link);
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

void updateSpriteIfNeeded(SDL_Renderer *renderer, SPRITE sprite, int direction, POSITION position, int *frame, int loop)
{
    if (loop%ANIMATION_LOOP == 0) 
    {
        updateSprite(renderer, sprite, direction, position, frame);
    }
    else
    {
        displaySprite(renderer, sprite, direction, position, frame);
    }
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

SDL_Texture *renderWidgetText(char *message, SDL_Color* color, int fontSize, SDL_Renderer *renderer, SDL_Rect *dstrect)
{
    // Color
    if (color == NULL)
    {
        color = malloc(sizeof(SDL_Color));
        color->r = 255;
        color->g = 255;
        color->b = 255;
    }
    color->a = 255;
    // Open the font
    TTF_Font *font = TTF_OpenFont(FONT, fontSize); // Open the font you want
    if (font == NULL)
    {
        SDL_ExitWithError("SDL || TTF_OpenFont");
        return NULL;
    }
    // Render the text to a surface
    SDL_Surface *surf = TTF_RenderText_Solid(font, message, *color);
    if (surf == NULL)
    {
        TTF_CloseFont(font);
        SDL_ExitWithError("SDL || TTF_RenderText");
        return NULL;
    }
    // Create a texture from the surface
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
    // Return the texture
    return texture;
}

void displayNumber(SDL_Renderer *renderer, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect)
{
    char message[255];
    dstrect->x -= (numberOfDigit(abs(number)) - 1) * (fontSize)/2;
    sprintf(message, "%d", number);
    SDL_Texture *texture = renderWidgetText(message, color, fontSize, renderer, dstrect);
    SDL_RenderCopy(renderer, texture, NULL, dstrect);
    SDL_DestroyTexture(texture);
}

void displayTextAndNumber(SDL_Renderer *renderer, char *message, int number, SDL_Color* color, int fontSize, SDL_Rect *dstrect)
{
    char message2[255];
    sprintf(message2, "%s %d", message, number);
    SDL_Texture *texture = renderWidgetText(message2, color, fontSize, renderer, dstrect);
    SDL_RenderCopy(renderer, texture, NULL, dstrect);
    SDL_DestroyTexture(texture);
}

void displayText(SDL_Renderer *renderer, char *message, SDL_Color* color, int fontSize, SDL_Rect *dstrect)
{
    SDL_Texture *texture = renderWidgetText(message, color, fontSize, renderer, dstrect);
    SDL_RenderCopy(renderer, texture, NULL, dstrect);
    SDL_DestroyTexture(texture);
}

int numberOfDigit(int number)
{
    if (number == 0)
    {
        return 1;
    }
    int count = 0;
    while (number != 0)
    {
        number /= 10;
        count++;
    }
    return count;
}

SDL_Rect createRect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

// SDL Helper functions

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
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
    if (SDL_Init(SDL_INIT_EVERYTHING|SDL_INIT_NOPARACHUTE) != 0)
    {
        SDL_ExitWithError("SDL | Failed to initialize");
    }
    if (TTF_Init() != 0)
    {
        SDL_ExitWithError("SDL | TTF: Failed to initialize");
    }

    *window = SDL_CreateWindow("Dungeon Dash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

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