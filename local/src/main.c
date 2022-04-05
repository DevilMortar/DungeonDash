#include "./kernel/header.h"

int main()
{
    // Start
    system("clear");
    srand(time(NULL));

    /* --------------------------------------- */

    // Initialisation SDL
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_initGameView(&window, &renderer);

    // Initialisation du menu
    BUTTON *buttonList = NULL;
    buttonList = createButton(renderer, "asset/texture/button/play_button.png", buttonList, 300, 100, WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 190, play, 1, -1, 52, 15);
    buttonList = createButton(renderer, "asset/texture/button/skin_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 + 190, skin, 1, -1, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/leave_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2 + 190, leave, 1, -1, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/confirm_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 70, confirm, 1, -2, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/left_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 - 50, left, 1, -2, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/right_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 - 50, right, 1, -2, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/locker.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50, locker, 1, -2, 32, 32);

    SKIN *skinList = NULL;
    skinList = createSkin(renderer, "asset/texture/player/4.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/3.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 30, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/2.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 20, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/1.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 1, 0, 32, 32);
    closeSkinList(skinList);

    //  Initialisation Game et Score
    GAME *game = malloc(sizeof(GAME));
    initGame(renderer, game);

    // Initialisation Player
    PLAYER *player;
    player = setPlayer(2);

    // Initialisation Fireball
    LIST_OBSTACLE fireball;
    fireball.sprite = newSprite(renderer, "asset/texture/fireball.png", 3, 74, 74, SPRITE_SIZE);
    fireball.warning = newSprite(renderer, "asset/texture/warning.png", 3, 32, 32, SPRITE_SIZE);
    fireball = setListObstacle(fireball);

    // Initialisation Hole
    int Hole[5][5] = {{0}};

    // Initialisation Coin
    COIN *coin = malloc(sizeof(COIN));
    coin->sprite = newSprite(renderer, "asset/texture/coin.png", 16, 32, 32, SPRITE_SIZE);
    coin->position.direction = 0;

    // Création de la texture de la grille et des trous
    TEXTURE map = newTexture(renderer, "asset/texture/board.bmp", MAP_SIZE, MAP_SIZE);
    TEXTURE hole = newTexture(renderer, "asset/texture/hole.png", MAP_SIZE, MAP_SIZE);
    TEXTURE title = newTexture(renderer, "asset/texture/title.png", 400, 150);
    title.dstrect.y = WINDOW_HEIGHT / 2 - 330;

    SDL_bool program_launched = SDL_TRUE;
    printf("\nGame statut | Game Initialized !\n");

    /* --------------------------------------- */
    while (program_launched)
    {
        int n = menu(buttonList, skinList, renderer, map, title, 50, 20);
        if (n > -1)
        {
            player = setPlayer(n);
            setPlayerSprite(renderer, player, skinList);
            startGame(window, renderer, game, player, fireball, Hole, coin, map, hole);
        }
        else
        {
            program_launched = SDL_FALSE;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}