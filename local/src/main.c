#include "../include/header.h"

int main(int argc, char *argv[])
{
    // Start
    srand(time(NULL));

    /* --------------------------------------- */

    // Initialisation SDL
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_initGameView(&window, &renderer);
    
    // Initialisation du menu
    BUTTON *buttonList = NULL;
    buttonList = createButton(renderer, "asset/texture/button/play_button.png", buttonList, 300, 100, WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 190, play, mainMenu, 52, 15, false);
    buttonList = createButton(renderer, "asset/texture/button/skin_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 + 190, skin, mainMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/leave_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2 + 190, leave, mainMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/confirm_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 70, confirm, skinMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/left_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 - 50, left, skinMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/right_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 - 50, right, skinMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/locker.png", buttonList, 70, 70, WINDOW_WIDTH / 2 - 35, WINDOW_HEIGHT / 2 - 7, locker, skinMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/back_menu.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, 2*WINDOW_HEIGHT / 3, backToMenu, gameOverMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/back_button.png", buttonList, 80, 80, 0, 0, reset, mainMenu, 32, 32, false);
    buttonList = createButton(renderer, "asset/texture/button/sound_button.png", buttonList, 80, 80, WINDOW_WIDTH-80, 0, sound, mainMenu, 32, 32, false);

    SKIN *skinList = NULL;
    skinList = createSkin(renderer, "asset/texture/player/6.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 32, 32);
    skinList = createSkin(renderer, "asset/texture/player/5.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 32, 32);
    skinList = createSkin(renderer, "asset/texture/player/4.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 50, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/3.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, 30, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/2.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 0, -2, 64, 64);
    skinList = createSkin(renderer, "asset/texture/player/1.png", skinList, 120, 120, WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 2 - 60, 1, 0, 32, 32);
    SKIN *firstSkin = skinList;

    //  Initialisation Game et Score
    GAME *game = malloc(sizeof(GAME));
    initGame(renderer, game);

    //Récupération de la sauvegarde
    recupData(firstSkin, game);
    game->money = 10000000;

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

    game->background.dstrect.y = 0;
    game->background.dstrect.x = 0;
    
    game->title.dstrect.y = game->titleSkin.dstrect.y = WINDOW_HEIGHT / 2 - 330;

    // Création des sons
    printf("%d\n", SL_getNumberOfSounds());
    char *soundName[7] = {"fire", "fire_2", "coin", "death", "play", "step", "warn"};
    SL_SOUND *soundList = SL_initSoundLib(7, "asset/sound/", soundName);
    Mix_Music *musique = Mix_LoadMUS("asset/sound/music.wav");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    printf("%d\n", SL_getNumberOfSounds());


    printf("\nGame statut | Game Initialized !\n");
    printf("\n------------------------------------------------------\n");
    /* --------------------------------------- */
    while (game->program_launched)
    {
        if (SL_isPlaying())
        {
            Mix_PlayMusic(musique, -1);
        }
        if(startMenu(buttonList, skinList, firstSkin, renderer, game, &player->skin)>0)
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
    printf("Game statut | Game is shutting down !\n");
    // DATA
    printf("Game statut | Prepare to save data...\n");
    saveData(firstSkin, game);
    printf("Game statut | Data saved !\n");
    // Free sound
    printf("Game statut | Preparing to free memory...\n");
    SL_freeSoundLib(soundList);
    freeButtons(buttonList);
    freeSkinList(skinList);
    SDL_DestroyTexture(game->map.texture);
    SDL_DestroyTexture(game->hole.texture);
    SDL_DestroyTexture(game->title.texture);
    SDL_DestroyTexture(game->titleSkin.texture);
    SDL_DestroyTexture(game->background.texture);
    SDL_DestroyTexture(game->gameOver.texture);
    free(game);
    free(player);
    free(coin);
    printf("Game statut | Memory has been freed with success !\n");
    // SDL QUIT
    printf("Game statut | Preparing to quit...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Mix_Quit();
    printf("Game statut | Game is now closed !\n");
}
