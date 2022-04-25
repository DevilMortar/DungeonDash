#include "header.h"

int startGame(SDL_Window *window, SDL_Renderer *renderer, GAME *game, PLAYER *player, LIST_OBSTACLE fireball, int Hole[5][5], COIN *coin, TEXTURE map, TEXTURE hole, SONG *songList, BUTTON *buttonList)
{
    // Set
    init(player, &fireball, Hole, coin, renderer, game);
    enum functions options = none;
    game->game_launched = SDL_TRUE;
    game->status = 0;

    // FrameLimiter
    Uint32 frameStart;
    unsigned int frameTime;

    // Game Loop
    while (game->game_launched)
    {
        // FPS check
        frameStart = SDL_GetTicks();

        // Contrôle
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            resetButtonState(buttonList);
            checkOverButtons(buttonList, options, event.motion.x, event.motion.y);
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (game->status == 1)
                {
                    options = checkClickButtons(buttonList, options, event.motion.x, event.motion.y);
                    switch (options)
                    {
                    case backToMenu:
                        game->game_launched = SDL_FALSE;
                        break;
                    }
                }
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
                    playSong(songList, "death");
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
                    game->game_launched = SDL_FALSE;
                    game->program_launched = SDL_FALSE;
                    return -1;
                    break;

                default:
                    continue;
                }
            case SDL_QUIT:
                game->game_launched = SDL_FALSE;
                game->program_launched = SDL_FALSE;
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
                    if (temp->warning <= 0 && game->status == 0)
                    {
                        if (detectColision(player->position, temp->position))
                        {
                            if (game->score > game->best)
                            {
                                game->best = game->score;
                            }
                            game->money += game->score;
                            game->status = 1;
                            playSong(songList, "death");
                        }
                    }
                    int output = updateFireball(temp);
                    if (output == 1)
                    {
                        fireball = deleteFromQueue(fireball);
                    }
                    else if (output == 2)
                    {
                        playSong(songList, "fire");
                    }
                    else if (output == 3)
                    {
                        playSong(songList, "fire_2");
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
                playSong(songList, "coin");
                coin->position = randomTeleport(coin->position, Hole);
                game->score += 1;
                printf("Game statut | Score : %d\n", game->score);
            }
        }

        // FPS
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);

        // Affichage
        displayGame(renderer, player, map, fireball, coin, Hole, hole, game, buttonList);
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
}
