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
    buttonList = createButton(renderer, "asset/texture/button/play_button.png", buttonList, 300, 100, WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 190, play, 1, mainmenu, 52, 15);
    buttonList = createButton(renderer, "asset/texture/button/skin_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 + 190, skin, 1, mainmenu, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/leave_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2 + 190, leave, 1, mainmenu, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/confirm_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 70, confirm, 1, skin, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/left_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 - 50, left, 1, skin, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/right_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 - 50, right, 1, skin, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/locker.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 - 50, locker, 1, skin, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/back_menu.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, 2*WINDOW_HEIGHT / 3, backToMenu, 1, none, 32, 32);
    buttonList = createButton(renderer, "asset/texture/button/back_button.png", buttonList, 80, 80, 0, 0, reset, 1, menu, 32, 32);

    SKIN *skinList = NULL;
    skinList = createSkin(renderer, "asset/texture/player/6.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 32, 32);
    skinList = createSkin(renderer, "asset/texture/player/5.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 32, 32);
    skinList = createSkin(renderer, "asset/texture/player/4.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/3.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 30, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/2.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 20, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/1.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 1, 0, 32, 32);
    SKIN *firstSkin = skinList;

    //  Initialisation Game et Score
    GAME *game = malloc(sizeof(GAME));
    initGame(renderer, game);

    //Récupération de la sauvegarde
    resetData(firstSkin, game);
    recupData(firstSkin, game);

    // Initialisation Player
    PLAYER *player;
    player = setPlayer(1);

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
    game->gameOver = newTexture(renderer, "asset/texture/game_over.png", 400, 50);
    game->gameOver.dstrect.y = 350;
    game->map = newTexture(renderer, "asset/texture/board.png", MAP_SIZE, MAP_SIZE);
    game->hole = newTexture(renderer, "asset/texture/hole.png", MAP_SIZE, MAP_SIZE);
    game->title = newTexture(renderer, "asset/texture/title.png", 700, 110);
    game->titleSkin = newTexture(renderer, "asset/texture/title_hero.png", 700, 110);
    game->background = newTexture(renderer, "asset/texture/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);
    game->boat = newTexture(renderer, "asset/texture/boat.png", WINDOW_WIDTH, MAP_SIZE*3.1);
    
    game->boat.dstrect.y = WINDOW_HEIGHT/2 - MAP_SIZE*1.8;
    game->boat.dstrect.x = WINDOW_WIDTH/2 - MAP_SIZE*1.83;

    game->background.dstrect.y = 0;
    game->background.dstrect.x = 0;
    
    game->title.dstrect.y = game->titleSkin.dstrect.y = WINDOW_HEIGHT / 2 - 330;

    TEXTURE unclickable_button = newTexture(renderer, "asset/texture/button/unclikable_button.png", 32, 32);

    // Création des sons
    char *soundName[4] = {"fire", "fire_2", "coin", "death"};
    SL_SOUND *soundList = SL_initSoundLib(4, "asset/sound/", soundName);
    /*
    soundList = loadSongInQueue(soundList, "asset/sound/fire.wav", "fire", 0);
    soundList = loadSongInQueue(soundList, "asset/sound/fire_2.wav", "fire_2", 1);
    soundList = loadSongInQueue(soundList, "asset/sound/death.wav", "death", 2);
    soundList = loadSongInQueue(soundList, "asset/sound/coin.wav", "coin", 3);
    */

    printf("\nGame statut | Game Initialized !\n");
    /* --------------------------------------- */
    while (game->program_launched)
    {
        if(menu(buttonList, skinList, firstSkin, renderer, game, &player->skin)>0)
        {
            setPlayer(player->skin);
            setPlayerSprite(renderer, player, skinList);
            startGame(window, renderer, game, player, fireball, Hole, coin, soundList, buttonList);
        }
        else
        {
            game->program_launched = SDL_FALSE;
        }
    }
    saveData(firstSkin, game);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
