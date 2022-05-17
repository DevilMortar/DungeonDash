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
    buttonList = createButton(renderer, "../asset/texture/button/play_button.png", buttonList, 300, 100, WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 190, play, mainMenu, 52, 15);
    buttonList = createButton(renderer, "../asset/texture/button/skin_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 + 190, skin, mainMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/leave_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 150, WINDOW_HEIGHT / 2 + 190, leave, mainMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/confirm_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 70, confirm, skinMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/left_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 170, WINDOW_HEIGHT / 2 - 50, left, skinMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/right_button.png", buttonList, 100, 100, WINDOW_WIDTH / 2 + 70, WINDOW_HEIGHT / 2 - 50, right, skinMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/locker.png", buttonList, 70, 70, WINDOW_WIDTH / 2 - 35, WINDOW_HEIGHT / 2 - 7, locker, skinMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/back_menu.png", buttonList, 100, 100, WINDOW_WIDTH / 2 - 50, 2*WINDOW_HEIGHT / 3, backToMenu, gameOverMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/back_button.png", buttonList, 80, 80, 0, 0, reset, mainMenu, 32, 32);
    buttonList = createButton(renderer, "../asset/texture/button/sound_button.png", buttonList, 80, 80, WINDOW_WIDTH-80, 0, sound, mainMenu, 32, 32);

    LIST_SKIN *skinList = NULL;
    skinList = malloc(sizeof (LIST_SKIN));
    skinList->first = NULL;
    skinList->last = NULL;
    skinList = createSkin(renderer, "../asset/texture/player/6.png", skinList, 0, 50, 32, 32);
    skinList = createSkin(renderer, "../asset/texture/player/5.png", skinList, 0, 50, 32, 32);
    skinList = createSkin(renderer, "../asset/texture/player/4.png", skinList, 0, 50, 64, 64);
    skinList = createSkin(renderer, "../asset/texture/player/3.png", skinList, 0, 30, 64, 64);
    skinList = createSkin(renderer, "../asset/texture/player/2.png", skinList, 0, -2, 64, 64);
    skinList = createSkin(renderer, "../asset/texture/player/1.png", skinList, 1, 0, 32, 32);

    //  Initialisation Game et Score
    GAME *game = malloc(sizeof(GAME));
    initGame(renderer, game);

    //Récupération de la sauvegarde
    recupData(skinList, game);

    // Initialisation Player
    PLAYER *player = malloc(sizeof(PLAYER));
    setPlayer(player, *skinList->first);

    // Initialisation Fireball
    LIST_OBSTACLE fireball;
    fireball.sprite = newSprite(renderer, "../asset/texture/fireball.png", 3, 74, 74, SPRITE_SIZE);
    fireball.warning = newSprite(renderer, "../asset/texture/warning.png", 3, 32, 32, SPRITE_SIZE);
    setListObstacle(&fireball);

    // Initialisation Hole
    int Hole[5][5] = {{0}};

    // Initialisation Coin
    COIN *coin = malloc(sizeof(COIN));
    coin->sprite = newSprite(renderer, "../asset/texture/coin.png", 16, 32, 32, SPRITE_SIZE);
    coin->position.direction = 0;

    // Création de la texture de la grille et des trous
    game->gameOver = newTexture(renderer, "../asset/texture/game_over.png", 400, 50);
    game->gameOver.dstrect.y = 350;
    game->map = newTexture(renderer, "../asset/texture/board.png", MAP_SIZE, MAP_SIZE);
    game->hole = newTexture(renderer, "../asset/texture/hole.png", MAP_SIZE, MAP_SIZE);
    game->title = newTexture(renderer, "../asset/texture/title.png", 350, 350);
    game->titleSkin = newTexture(renderer, "../asset/texture/title_hero.png", 700, 110);
    game->background = newTexture(renderer, "../asset/texture/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);

    game->background.dstrect.y = 0;
    game->background.dstrect.x = 0;
    
    game->titleSkin.dstrect.y = WINDOW_HEIGHT / 2 - 330;
    game->title.dstrect.y = 100;

    // Création des sons
    SL_initSoundLib("../asset/sound/", 50, false);
    Mix_Music *musique = Mix_LoadMUS("../asset/sound/music.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME * 90/100);

    printf("\nInitializing | Game Initialized !\n");
    printf("\n------------------------------------------------------\n");

    // Intro
    SDL_RenderClear(renderer);
    SL_playSong("intro", 100);
    SL_playSong("intro_2", 100);
    SL_playSong("intro_drum", 100);
    SDL_RenderCopy(renderer, game->title.texture, NULL, &game->title.dstrect);
    SDL_RenderPresent(renderer);
    game->title.dstrect.y = WINDOW_HEIGHT / 2 - game->title.dstrect.h / 2;
    for (int i = 0; i < 51; i++) {
        SDL_SetTextureAlphaMod(game->title.texture, i*5);
        SDL_RenderCopy(renderer, game->title.texture, NULL, &game->title.dstrect);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
    for (int i = 255; i > 0; i--) {
        SDL_SetTextureAlphaMod(game->title.texture, i);
        SDL_RenderCopy(renderer, game->title.texture, NULL, &game->title.dstrect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        SDL_RenderClear(renderer);
    }
    game->title.dstrect.y = 100;
    SDL_SetTextureAlphaMod(game->title.texture, 255);




    /* --------------------------------------- */
    while (game->program_launched)
    {
         if (SL_isPlaying())
        {
            Mix_PlayMusic(musique, -1);
        }
        if(startMenu(buttonList, skinList, player, renderer, game) > 0) {
            startGame(window, renderer, game, player, fireball, Hole, coin, buttonList);
        } else {
            game->program_launched = SDL_FALSE;
        }
    }
    printf("\nGame status | Game is shutting down !\n");
    // DATA
    printf("Exit | Prepare to save data...\n");
    saveData(skinList, game);
    printf("Exit | Data saved !\n");
    // Free sound
    printf("Exit | Preparing to free memory...\n");
    SL_freeSoundLib();
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
    printf("Exit | Memory has been freed with success !\n");
    // SDL QUIT
    printf("Exit | Preparing to quit...\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Mix_Quit();
    printf("Exit | Game is now closed !\n");
}
