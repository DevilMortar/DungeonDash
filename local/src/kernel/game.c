#include "header.h"

int startGame(SDL_Window *window, SDL_Renderer *renderer, GAME *game, PLAYER *player, LIST_OBSTACLE fireball, int Hole[5][5], COIN *coin, SL_SOUND *soundList, BUTTON *buttonList)
{
    // Set
    init(player, &fireball, Hole, coin, renderer, game);
    enum functions options = none;
    game->menu = gameLaunched;

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
            checkOverButtons(buttonList, options, game->menu, event.motion.x, event.motion.y);
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (game->status == 1)
                {
                    options = checkClickButtons(buttonList, options, game->menu, event.motion.x, event.motion.y);
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
                            game->menu = gameOverMenu;
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

int displayGame(SDL_Renderer *renderer, PLAYER *player, LIST_OBSTACLE fireball, COIN *coin, int Hole[5][5], GAME *game, BUTTON * buttonList)
{
    SDL_RenderCopy(renderer, game->background.texture, NULL, &game->background.dstrect);
    SDL_RenderCopy(renderer, game->map.texture, NULL, &game->map.dstrect);
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            if (Hole[y][x] == 1)
            {
                POSITION temp;
                temp.x = CASE_OFFSET_X + x * CASE_SIZE;
                temp.y = CASE_OFFSET_Y + y * CASE_SIZE;
                game->hole = updateTexture(renderer, game->hole, temp, SPRITE_SIZE, SPRITE_SIZE);
            }
        }
    }
    if (game->loop % ANIMATION_LOOP == 0)
    {
        if (game->status == 0)
        {
            POSITION scorecoin = {WINDOW_WIDTH / 2 - 20 - numberOfDigit(game->money) * SCORE_SIZE/2, 10};
            updateSprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
            updateSprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
            updateSprite(renderer, game->scoreCoin, 0, scorecoin, &game->scoreCoin.frame);
        }
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning > 0)
                {
                    updateSprite(renderer, fireball.warning, 0, temp->position, &temp->frame);
                }
                else
                {
                    updateSprite(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame);
                }
                temp = temp->next;
            }
        }
    }
    else
    {
        if (game->status == 0)
        {
            POSITION scorecoin = {WINDOW_WIDTH / 2 - 20 - numberOfDigit(game->money) * SCORE_SIZE/2, 10};
            displaySprite(renderer, player->sprite, player->position.direction, player->position, &player->sprite.frame);
            displaySprite(renderer, coin->sprite, coin->position.direction, coin->position, &coin->sprite.frame);
            displaySprite(renderer, game->scoreCoin, 0, scorecoin, &game->scoreCoin.frame);
        }
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning > 0)
                {
                    displaySprite(renderer, fireball.warning, 0, temp->position, &temp->frame);
                }
                else
                {
                    displaySprite(renderer, fireball.sprite, temp->position.direction, temp->position, &temp->frame);
                }
                temp = temp->next;
            }
        }
    }
    if (game->status == 0) {
        char scorestr[WIDGET_LENGTH];
        sprintf(scorestr, "%d", game->score);
        SDL_Texture *scoreCoin = renderWidgetText(scorestr, NULL, SCORE_SIZE, renderer, &game->coinrect);
        game->coinrect.x = WINDOW_WIDTH / 2 - (numberOfDigit(game->money)-1) * SCORE_SIZE/2;
        SDL_RenderCopy(renderer, scoreCoin, NULL, &game->coinrect);
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
            game->endscreen.dstrect.x = WINDOW_WIDTH/2 - game->endscreen.dstrect.w/2;
            game->endscreen.dstrect.y = WINDOW_HEIGHT/2 - game->endscreen.dstrect.h/2;  
            SDL_RenderCopy(renderer, game->endscreen.texture, NULL, &game->endscreen.dstrect);
            SDL_RenderCopy(renderer, scoreprint, NULL, &game->endscorerect);
            SDL_RenderCopy(renderer, recordprint, NULL, &game->endbestrect);
            SDL_RenderCopy(renderer, game->gameOver.texture, NULL, &game->gameOver.dstrect);
            displayButtons(renderer, buttonList, gameOverMenu);
        }
    }
    return 0;
}
