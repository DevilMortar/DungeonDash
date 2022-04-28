#include "header.h"

int startGame(SDL_Window *window, SDL_Renderer *renderer, GAME *game, PLAYER *player, LIST_OBSTACLE fireball, int Hole[5][5], COIN *coin, SL_SOUND *soundList, BUTTON *buttonList)
{
    // Set
    init(player, &fireball, Hole, coin, renderer, game);
    enum functions options = none;

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
                    player->directionPressed = 1;
                    continue;
                case SDLK_s:
                    player->directionPressed = 3;
                    continue;
                case SDLK_q:
                    player->directionPressed = 4;
                    continue;
                case SDLK_d:
                    player->directionPressed = 2;
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
                    game->game_launched = SDL_FALSE;
                    game->program_launched = SDL_FALSE;
                    return -1;
                    break;

                default:
                    continue;
                }
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_z:
                    if (player->directionPressed == 1)
                        player->directionPressed = 0;
                    continue;
                case SDLK_s:
                    if (player->directionPressed == 3)
                        player->directionPressed = 0;   
                    continue;
                case SDLK_q:
                    if (player->directionPressed == 4)
                        player->directionPressed = 0;
                    continue;
                case SDLK_d:
                    if (player->directionPressed == 2)
                        player->directionPressed = 0;
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
        if (game->status == 0 && player->move == 0 && player->directionPressed != 0) {
            if (checkMovePlayer(player, player->directionPressed, Hole)) {
                player->move = PLAYER_MOVE;
                player->position.direction = player->directionPressed;
                // Play sound jump
                SL_playSong(soundList, "jump");
            }
        }
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
                            SL_playSong(soundList, "death");
                        }
                    }
                    int output = updateFireball(temp);
                    if (output == 1)
                    {
                        fireball = deleteFromQueue(fireball);
                    }
                    else if (output == 2)
                    {
                        SL_playSong(soundList, "fire");
                    }
                    else if (output == 3)
                    {
                        SL_playSong(soundList, "fire_2");
                    }
                }
            }
            if (temp != NULL)
                temp = temp->next;
        } while (temp != NULL);
        if ((double)game->loop >= 100 * exp((-(float)game->score) / 40) && game->status == 0 && game->score > 0)
        {
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
                SL_playSong(soundList, "coin");
                coin->position = randomTeleport(coin->position, Hole);
                game->score += 1;
                printf("Game statut | Score : %d\n", game->score);
            }
        }

        // FPS
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);

        // Affichage
        displayGame(renderer, player, fireball, coin, Hole, game, buttonList);
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
