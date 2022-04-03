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

    /* --------------------------------------- */

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

    // Set
    SDL_bool program_launched = SDL_TRUE;
    game->status = 0;

    // FrameLimiter
    Uint32 frameStart;
    unsigned int frameTime;

    // Game Loop
    while (program_launched)
    {
        // FPS check
        frameStart = SDL_GetTicks();

        // Contrôle
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                printf("Event listener | Click detected in %d / %d\n", event.motion.x, event.motion.y);
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_z:
                    if (game->status == 0 && player->move == 0)
                        checkMovePlayer(player, 1, Hole);
                    continue;
                case SDLK_s:
                    if (game->status == 0 && player->move == 0)
                        checkMovePlayer(player, 3, Hole);
                    continue;
                case SDLK_q:
                    if (game->status == 0 && player->move == 0)
                        checkMovePlayer(player, 4, Hole);
                    continue;
                case SDLK_d:
                    if (game->status == 0 && player->move == 0)
                        checkMovePlayer(player, 2, Hole);
                    continue;
                case SDLK_f:
                    printf("Admin | You spawned a fireball\n");
                    fireball = newObstacle(fireball);
                    continue;
                case SDLK_r:
                    printf("Admin | Restart completed ! \n");
                    init(player, &fireball, Hole, coin, renderer, game);
                    continue;
                case SDLK_o:
                    printf("Admin | Fullscreen activated ! \n");
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    continue;
                case SDLK_p:
                    printf("Admin | Fullscreen desactivated ! \n");
                    SDL_SetWindowFullscreen(window, 0);
                    continue;
                case SDLK_m:
                    printf("Admin | Force quit ! \n");
                    program_launched == SDL_FALSE;
                    break;

                default:
                    continue;
                }
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;
            default:
                break;
            }
        }

        // Player Movement
        if (player->move > 0)
        {
            movePlayer(player, player->position.direction, Hole);
        }

        // Fireball
        OBSTACLE *temp = fireball.first;
        do
        {
            if (game->loop % FIREBALL_MOVELOOP == 0 && game->loop != 0 && game->score > 0)
            {
                if (fireball.first != NULL)
                {
                    if (temp->warning == 0 && game->status == 0)
                    {
                        if (detectColision(player->position, temp->position))
                        {
                            game->status = 1;
                        }
                    }
                    if (updateFireball(temp))
                    {
                        fireball = deleteFromQueue(fireball);
                    }
                }
            }
            if (temp != NULL)
                temp = temp->next;
        } while (temp != NULL);
        if ((double)game->loop >= 100 * exp((-(float)game->score) / 40) && game->status == 0 && game->score > 0)
        {
            printf("%f\n", 100 * exp((-(float)game->score) / 40));
            fireball = newObstacle(fireball);
            game->loop = 0;
        }
        else
            game->loop += 1;

        // Collision
        if (game->status == 0)
        {
            if (detectColision(player->position, coin->position))
            {
                coin->position = randomTeleport(coin->position, Hole);
                game->score += 1;
                printf("Game statut | Score : %d\n", game->score);
            }
        }

        // FPS
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);

        // Affichage
        displayGame(renderer, player, map, fireball, coin, Hole, hole, game);
        SDL_RenderPresent(renderer);
        if (SDL_RenderClear(renderer) != 0)
        {
            SDL_ExitWithError("SDL | Failed to clear renderer");
        }
    }

    while (fireball.first != NULL)
    {
        fireball = deleteFromQueue(fireball);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}