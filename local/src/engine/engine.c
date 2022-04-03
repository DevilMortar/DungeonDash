#include "engine.h"

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void initGame(SDL_Renderer * renderer, GAME *game) {
    game->endscreen = newTexture(renderer, "asset/texture/end.png", 400, 200);
    game->deathAnimation = newSprite(renderer, "asset/texture/skull.png", 18, 196, 196, SPRITE_SIZE);
    game->scoreCoin = newSprite(renderer, "asset/texture/coin.png", 16, 32, 32, SCORE_SIZE);
    game->endscorerect.x = WINDOW_WIDTH / 2 - 100;
    game->endscorerect.y = WINDOW_HEIGHT / 2 - END_SCORE_SIZE;
    game->endbestrect.x = WINDOW_WIDTH / 2 - 100;
    game->endbestrect.y = WINDOW_HEIGHT / 2 + END_SCORE_SIZE;
    game->coinrect.x = WINDOW_WIDTH / 2 + 5;
    game->coinrect.y = 8;
}

int init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer, GAME * game)
{
    // Initialisation du joueur
    setPlayer(2);
    setPlayerSprite(renderer, player);

    // Initialisation du jeu
    game->status = 0;
    game->deathAnimation.frame = 0;
    game->loop = 0;
    game->score = 0;

    // Initialisation des boules de feu
    while (fireball->first != NULL) {
        *fireball = deleteFromQueue(*fireball);
    }
    *fireball = setListObstacle(*fireball);
    for (int i=0; i<25; i++) {
        *(Hole + i) = 0;
    }

    // Initialisation des trous
    int ran = 0;
    for (int i=0; i<2; i++) {
    do {
        ran = rand()%25;
    } while (*(Hole + ran) == 1 || ran==0 || ran==4 || ran==20 || ran==24);
    *(Hole + ran) = 1;
    }

    // Initialisation de la pièce
    coin->position = randomTeleport(coin->position, Hole);

    printf("\nGame statut | Game started !\n");
    return 0;
}

PLAYER *setPlayer(int skin)
{
    PLAYER *player = malloc(sizeof(PLAYER));
    player->position.x = CASE_OFFSET_X;
    player->position.y = CASE_OFFSET_Y;
    player->move = 0;
    player->skin = skin;
    return player;
}

void checkMovePlayer(PLAYER *player, int direction, int Hole[5][5]) {
    player->position.direction = direction;
    switch (direction)
    {
    case 1:
        if (player->position.y > CASE_OFFSET_Y && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE - 1][(player->position.x - CASE_OFFSET_X) / CASE_SIZE] != 1)
        {
            player->move = PLAYER_MOVE;
        }
        break;
    case 2:
        if (player->position.x < 4 * CASE_SIZE + CASE_OFFSET_X && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE][(player->position.x - CASE_OFFSET_X) / CASE_SIZE + 1] != 1)
        {
            player->move = PLAYER_MOVE;
        }
        break;
    case 3:
        if (player->position.y < 4 * CASE_SIZE + CASE_OFFSET_Y && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE + 1][(player->position.x - CASE_OFFSET_X) / CASE_SIZE] != 1)
        {
            player->move = PLAYER_MOVE;
        }
        break;
    case 4:
        if (player->position.x > CASE_OFFSET_X && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE][(player->position.x - CASE_OFFSET_X) / CASE_SIZE - 1] != 1)
        {
            player->move = PLAYER_MOVE;
        }
        break;
    }
}

void movePlayer(PLAYER *player, int direction, int Hole[5][5])
{
    switch (direction)
    {
    case 1:
        player->position.y -= CASE_SIZE/PLAYER_MOVE;
        break;
    case 2:
        player->position.x += CASE_SIZE/PLAYER_MOVE;
        break;
    case 3:
        player->position.y += CASE_SIZE/PLAYER_MOVE;
        break;
    case 4:
        player->position.x -= CASE_SIZE/PLAYER_MOVE;
        break;
    }
    if (player->move > 0) {
        player->sprite.dstrect.w = player->sprite.dstrect.h = SPRITE_SIZE + player->move;
    } 
    player->move -= 1; 
}

LIST_OBSTACLE setListObstacle(LIST_OBSTACLE list)
{
    list.first = NULL;
    list.last = NULL;
    return list;
}

LIST_OBSTACLE newObstacle(LIST_OBSTACLE list)
{
    OBSTACLE *new = malloc(sizeof(OBSTACLE));
    int direction = rand() % 4 + 1;
    new->frame = 0;
    new->position.direction = direction;
    new->warning = OBSTACLE_WARNING;
    switch (direction)
    {
    case 1:
        new->position.y = 5 * CASE_SIZE + CASE_OFFSET_Y;
        new->position.x = rand() % 5 * CASE_SIZE + CASE_OFFSET_X;
        break;
    case 2:
        new->position.x = CASE_OFFSET_X - CASE_SIZE;
        new->position.y = rand() % 5 * CASE_SIZE + CASE_OFFSET_Y;
        break;
    case 3:
        new->position.y = CASE_OFFSET_Y - CASE_SIZE;
        new->position.x = rand() % 5 * CASE_SIZE + CASE_OFFSET_X;
        break;
    case 4:
        new->position.x = 5 * CASE_SIZE + CASE_OFFSET_X;
        new->position.y = rand() % 5 * CASE_SIZE + CASE_OFFSET_Y;
        break;
    }
    return addToQueue(list, new);
}

LIST_OBSTACLE addToQueue(LIST_OBSTACLE list, OBSTACLE *new)
{
    if (list.first == NULL)
    {
        list.first = new;
        list.last = new;
        new->previous = NULL;
        new->next = NULL;
    }
    else
    {
        list.first->previous = new;
        new->next = list.first;
        new->previous = NULL;
        list.first = new;
    }
    return list;
}

LIST_OBSTACLE deleteFromQueue(LIST_OBSTACLE list)
{
    if (list.first == list.last)
    {
        free(list.first);
        list.first = NULL;
        list.last = NULL;
    }
    else
    {
        OBSTACLE *temp = list.last->previous;
        free(list.last);
        list.last = temp;
        temp->next = NULL;
    }
    return list;
}

bool updateFireball(OBSTACLE *obstacle)
{
    if (obstacle->warning > 0)
    {
        obstacle->warning -= 1;
        return false;
    }
    else
    {
        switch (obstacle->position.direction)
        {
        case 1:
            if (obstacle->position.y < CASE_OFFSET_Y - CASE_SIZE)
            {
                return true;
            }
            else
            {
                obstacle->position.y -= FIREBALL_SPEED;
            }
            break;
        case 2:
            if (obstacle->position.x > CASE_OFFSET_X + 5 * CASE_SIZE)
            {
                return true;
            }
            else
            {
                obstacle->position.x += FIREBALL_SPEED;
            }
            break;
        case 3:
            if (obstacle->position.y > CASE_OFFSET_Y + 5 * CASE_SIZE)
            {
                return true;
            }
            else
            {
                obstacle->position.y += FIREBALL_SPEED;
            }
            break;
        case 4:
            if (obstacle->position.x < CASE_OFFSET_X - CASE_SIZE)
            {
                return true;
            }
            else
            {
                obstacle->position.x -= FIREBALL_SPEED;
            }
            break;
        }
    }
    return false;
}

bool detectColision(POSITION posplayer, POSITION posobject)
{
    int deltax = posplayer.x - posobject.x;
    int deltay = posplayer.y - posobject.y;
    int distance = sqrt(pow(deltax, 2) + pow(deltay, 2));
    if (distance < SPRITE_SIZE / 2)
    {
        printf("Game statut | Colision detected\n");
        return true;
    }
    else
    {
        return false;
    }
}

POSITION randomTeleport(POSITION position, int Hole[5][5])
{
    int x = 0;
    int y = 0;
    do
    {
        x = CASE_OFFSET_X + rand() % 5 * CASE_SIZE;
        y = CASE_OFFSET_Y + rand() % 5 * CASE_SIZE;
    } while ((position.x == x && position.y == y) || (Hole[(y - CASE_OFFSET_Y) / CASE_SIZE][(x - CASE_OFFSET_X) / CASE_SIZE]));
    position.x = x;
    position.y = y;
    return position;
}

int startGame(SDL_Window * window, SDL_Renderer *renderer, GAME * game, PLAYER * player, LIST_OBSTACLE fireball, int Hole[5][5],COIN * coin, TEXTURE map, TEXTURE hole)
{
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
}
