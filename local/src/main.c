#include "./engine/engine.h"

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

    // Initialisation Global
    init(player, &fireball, Hole, coin, renderer, game);

    // Création de la texture de la grille et des trous
    TEXTURE map = newTexture(renderer, "asset/texture/board.bmp", MAP_SIZE, MAP_SIZE);
    TEXTURE hole = newTexture(renderer, "asset/texture/hole.png", MAP_SIZE, MAP_SIZE);

    printf("\nGame statut | Game Initialized !\n");

    /* --------------------------------------- */
    startGame(window,renderer, game, player, fireball, Hole, coin, map, hole);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}