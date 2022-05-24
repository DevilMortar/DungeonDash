#include "../include/game.h"

int startGame(SDL_Window *window, SDL_Renderer *renderer, GAME *game, PLAYER *player, LIST_OBSTACLE fireball, int Hole[5][5], COIN *coin, BUTTON *buttonList)
{
    // Set
    init(player, &fireball, Hole, coin, renderer, game);
    enum functions options = none;
    game->menu = gameLaunched;
    BUTTON * buttonClicked = NULL;

    // FrameLimiter
    Uint32 frameStart;
    unsigned int frameTime;

    // Play song
    SL_playSong("play", VOLUME_BUTTON, 0);

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
            checkOverButtons(buttonList, options, game->menu, event.motion.x, event.motion.y);
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (game->status == 1)
                {
                    buttonClicked = checkClickButtons(buttonList, &options, game->menu, event.motion.x, event.motion.y);
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
                    newObstacle(&fireball);
                    continue;
                case SDLK_r:
                    printf("Admin | Restart completed ! \n");
                    init(player, &fireball, Hole, coin, renderer, game);
                    continue;
                case SDLK_m:
                    printf("Admin | Force quit ! \n");
                    game->game_launched = SDL_FALSE;
                    game->program_launched = SDL_FALSE;
                    return -1;
                    break;
                case SDLK_UP:
                    player->directionPressed = 1;
                    continue;
                case SDLK_DOWN:
                    player->directionPressed = 3;
                    continue;
                case SDLK_LEFT:
                    player->directionPressed = 4;
                    continue;
                case SDLK_RIGHT:
                    player->directionPressed = 2;
                    continue;
                case SDLK_ESCAPE:
                    game->game_launched = SDL_FALSE;
                    game->program_launched = SDL_FALSE;
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
                case SDLK_UP:
                    if (player->directionPressed == 1)
                        player->directionPressed = 0;
                    continue;
                case SDLK_DOWN:
                    if (player->directionPressed == 3)
                        player->directionPressed = 0;
                    continue;
                case SDLK_LEFT:
                    if (player->directionPressed == 4)
                        player->directionPressed = 0;
                    continue;
                case SDLK_RIGHT:
                    if (player->directionPressed == 2)
                        player->directionPressed = 0;
                    continue;
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
        if (game->status == 0 && player->move == 0 && player->directionPressed != 0)
        {
            if (checkMovePlayer(player, player->directionPressed, Hole))
            {
                player->move = PLAYER_MOVE;
                player->position.direction = player->directionPressed;
                // Play sound jump
                SL_playSong("step", VOLUME_STEP, 0);
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
                            game->menu = gameOverMenu;
                            SL_playSong("death", VOLUME_STEP, 0);
                        }
                    }
                    int output = updateFireball(temp);
                    if (output == 1)
                    {
                        deleteFromQueue(&fireball);
                    }
                    else if (output == 2)
                    {
                        SL_playSong("fire", VOLUME_FIREBALL, 0);
                    }
                    else if (output == 3)
                    {
                        SL_playSong("fire_2", VOLUME_FIREBALL, 0);
                    }
                    else if (output == 4)
                    {
                        SL_playSong("fire_3", VOLUME_FIREBALL, 0);
                    }
                }
            }
            if (temp != NULL)
                temp = temp->next;
        } while (temp != NULL);
        if ((double)game->loop >= DIFFICULTY_K * exp((-(float)game->score) / DIFFICULTY_T) && game->status == 0 && game->score > 0)
        {
            newObstacle(&fireball);
            SL_playSong("warn", VOLUME_STEP, 0);
            game->loop = 0;
        }
        else
            game->loop += 1;

        // Collision
        if (game->status == 0)
        {
            if (detectColision(player->position, coin->position))
            {
                SL_playSong("coin", VOLUME_COIN, 0);
                coin->position = randomTeleport(coin->position, Hole);
                game->score += 1;
            }
        }

        // Affichage
        displayGame(renderer, player, fireball, coin, Hole, game, buttonList);

        // FPS
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);
        SDL_RenderPresent(renderer);
        if (SDL_RenderClear(renderer) != 0)
        {
            SDL_ExitWithError("SDL | Failed to clear renderer");
        }
    }
    while (fireball.first != NULL)
    {
        deleteFromQueue(&fireball);
    }
}

int displayGame(SDL_Renderer *renderer, PLAYER *player, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], GAME *game, BUTTON *buttonList)
{
    SDL_RenderCopy(renderer, game->background.texture, NULL, &game->background.dstrect);
    SDL_RenderCopy(renderer, game->map.texture, NULL, &game->map.dstrect);
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            if (Hole[y][x] == 1)
            {
                POSITION temp = {CASE_OFFSET_X + x * CASE_SIZE, CASE_OFFSET_Y + y * CASE_SIZE};
                game->hole = updateTexture(renderer, game->hole, temp, SPRITE_SIZE, SPRITE_SIZE);
            }
        }
    }
    if (game->status == 0)
    {
        POSITION scorecoin = {WINDOW_WIDTH / 2 - 15 - numberOfDigit(game->score) * (MONEY_SIZE)/2, 10, 0};
        updateSpriteIfNeeded(renderer, player->skin.skin_sprite, player->position.direction, player->position, &player->skin.skin_sprite.frame, game->loop);
        updateSpriteIfNeeded(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame, game->loop);
        updateSpriteIfNeeded(renderer, game->scoreCoin, 0, scorecoin, &game->scoreCoin.frame, game->loop);
        SDL_Rect coinrect = {WINDOW_WIDTH / 2 + 5,8,0,0};
        displayNumber(renderer, game->score, NULL, SCORE_SIZE, &coinrect);
    }
    if (fireball.last != NULL)
    {
        OBSTACLE *temp = fireball.first;
        while (temp != NULL)
        {
            if (temp->warning > 0)
            {
                updateSpriteIfNeeded(renderer, fireball.warning, 0, temp->position, &temp->frame, game->loop);
            }
            else
            {
                updateSpriteIfNeeded(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame, game->loop);
            }
            temp = temp->next;
        }
    }
    if (game->status == 1)
    {
        char scorestr[WIDGET_LENGTH];
        sprintf(scorestr, "Score     %d", game->score);
        SDL_Texture *scoreprint = renderWidgetText(scorestr, NULL, END_SCORE_SIZE, renderer, &game->endscorerect);
        sprintf(scorestr, "Record   %d", game->best);
        SDL_Texture *recordprint = renderWidgetText(scorestr, NULL, END_SCORE_SIZE, renderer, &game->endbestrect);
        if (game->deathAnimation.frame < game->deathAnimation.max - 1)
        {
            updateSprite(renderer, game->deathAnimation, 0, player->position, &game->deathAnimation.frame);
        }
        else
        {
            displaySprite(renderer, game->deathAnimation, 0, player->position, &game->deathAnimation.frame);
            game->endscreen.dstrect.x = WINDOW_WIDTH / 2 - game->endscreen.dstrect.w / 2;
            game->endscreen.dstrect.y = WINDOW_HEIGHT / 2 - game->endscreen.dstrect.h / 2;
            SDL_RenderCopy(renderer, game->endscreen.texture, NULL, &game->endscreen.dstrect);
            SDL_RenderCopy(renderer, scoreprint, NULL, &game->endscorerect);
            SDL_RenderCopy(renderer, recordprint, NULL, &game->endbestrect);
            SDL_RenderCopy(renderer, game->gameOver.texture, NULL, &game->gameOver.dstrect);
            displayButtonList(renderer, buttonList, gameOverMenu);
        }
    }
    return 0;
}
