#include "../include/engine.h"

void initGame(SDL_Renderer *renderer, GAME *game)
{
    game->program_launched = SDL_TRUE;
    game->endscreen = newTexture(renderer, "../asset/texture/end.png", 400, 200);
    game->deathAnimation = newSprite(renderer, "../asset/texture/skull.png", 18, 196, 196, SPRITE_SIZE, SPRITE_SIZE);
    game->scoreCoin = newSprite(renderer, "../asset/texture/coin.png", 16, 32, 32, SCORE_SIZE, SCORE_SIZE);
    game->endscorerect.x = WINDOW_WIDTH / 2 - 100;
    game->endscorerect.y = WINDOW_HEIGHT / 2 - END_SCORE_SIZE;
    game->endbestrect.x = WINDOW_WIDTH / 2 - 100;
    game->endbestrect.y = WINDOW_HEIGHT / 2 + END_SCORE_SIZE;
    game->coinrect.x = WINDOW_WIDTH / 2 + 5;
    game->coinrect.y = 8;
}

int init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer, GAME *game)
{
    // Initialisation du jeu
    game->status = 0;
    game->deathAnimation.frame = 0;
    game->loop = 0;
    game->score = 0;
    game->game_launched = SDL_TRUE;

    // Initialisation du joueur
    player->position.x = CASE_OFFSET_X + 2 * CASE_SIZE;
    player->position.y = CASE_OFFSET_Y + 2 * CASE_SIZE;
    player->position.direction = 3;

    // Initialisation des boules de feu
    while (fireball->first != NULL)
    {
        deleteFromQueue(fireball);
    }
    setListObstacle(fireball);

    // Initialisation des trous
    for (int i = 0; i < 25; i++)
    {
        *(Hole + i) = 0;
    }
    int ran = 0;
    for (int i = 0; i < 2; i++)
    {
        do
        {
            ran = rand() % 12 + 12*i;
        } while (*(Hole + ran) == 1 || ran == 0 || ran == 4 || ran == 20 || ran == 24 || ran == 12);
        *(Hole + ran) = 1;
    }


    // Initialisation de la pièce
    coin->position = randomTeleport(coin->position, Hole);

    printf("\nGame status | Game started !\n");
    return 0;
}

void setPlayer(PLAYER *player, SKIN skin)
{
    player->position.x = CASE_OFFSET_X + 2 * CASE_SIZE;
    player->position.y = CASE_OFFSET_Y + 2 * CASE_SIZE;
    player->move = 0;
    player->skin = skin;
}

void setPlayerSprite(PLAYER *player, SKIN * skin)
{
    player->skin = *skin;
    player->skin.skin_sprite.dstrect.w = player->skin.skin_sprite.dstrect.h = SPRITE_SIZE;
}

SKIN * resetSkinSize(SKIN * skin) {
    skin->skin_sprite.dstrect.w = skin->skin_sprite.dstrect.h = SKIN_W_H;
    return skin;
}

bool checkMovePlayer(PLAYER *player, int direction, int Hole[5][5])
{
    player->position.direction = direction;
    switch (direction)
    {
    case 1:
        if (player->position.y > CASE_OFFSET_Y && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE - 1][(player->position.x - CASE_OFFSET_X) / CASE_SIZE] != 1)
        {
            return true;
        }
        break;
    case 2:
        if (player->position.x < 4 * CASE_SIZE + CASE_OFFSET_X && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE][(player->position.x - CASE_OFFSET_X) / CASE_SIZE + 1] != 1)
        {
            return true;
        }
        break;
    case 3:
        if (player->position.y < 4 * CASE_SIZE + CASE_OFFSET_Y && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE + 1][(player->position.x - CASE_OFFSET_X) / CASE_SIZE] != 1)
        {
            return true;
        }
        break;
    case 4:
        if (player->position.x > CASE_OFFSET_X && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE][(player->position.x - CASE_OFFSET_X) / CASE_SIZE - 1] != 1)
        {
            return true;
        }
        break;
    }
    return false;
}

void movePlayer(PLAYER *player, int direction, int Hole[5][5])
{
    switch (direction)
    {
    case 1:
        player->position.y -= CASE_SIZE / PLAYER_MOVE;
        break;
    case 2:
        player->position.x += CASE_SIZE / PLAYER_MOVE;
        break;
    case 3:
        player->position.y += CASE_SIZE / PLAYER_MOVE;
        break;
    case 4:
        player->position.x -= CASE_SIZE / PLAYER_MOVE;
        break;
    }
    if (player->move > 0)
    {
        player->skin.skin_sprite.dstrect.w = player->skin.skin_sprite.dstrect.h = SPRITE_SIZE + player->move;
    }
    player->move -= 1;
}

void setListObstacle(LIST_OBSTACLE *list)
{
    list->first = NULL;
    list->last = NULL;
}

void newObstacle(LIST_OBSTACLE *list)
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
    addToQueue(list, new);
}

void addToQueue(LIST_OBSTACLE *list, OBSTACLE *new)
{
    if (list->first == NULL) {
        list->first = new;
        list->last = new;
        new->previous = NULL;
        new->next = NULL;
    } else {
        list->first->previous = new;
        new->next = list->first;
        new->previous = NULL;
        list->first = new;
    }
}

void deleteFromQueue(LIST_OBSTACLE *list)
{
    if (list->first == list->last) {
        free(list->first);
        list->first = NULL;
        list->last = NULL;
    } else {
        OBSTACLE *temp = list->last->previous;
        free(list->last);
        list->last = temp;
        temp->next = NULL;
    }
}

int updateFireball(OBSTACLE *obstacle)
{
    if (obstacle->warning > 1)
    {
        obstacle->warning -= 1;
        return 0;
    }
    else if (obstacle->warning == 1)
    {
        obstacle->warning -= 1;
        return rand() % 2 + 2;
    }
    else
    {
        switch (obstacle->position.direction)
        {
        case 1:
            if (obstacle->position.y < CASE_OFFSET_Y - CASE_SIZE)
            {
                return 1;
            }
            else
            {
                obstacle->position.y -= FIREBALL_SPEED;
            }
            break;
        case 2:
            if (obstacle->position.x > CASE_OFFSET_X + 5 * CASE_SIZE)
            {
                return 1;
            }
            else
            {
                obstacle->position.x += FIREBALL_SPEED;
            }
            break;
        case 3:
            if (obstacle->position.y > CASE_OFFSET_Y + 5 * CASE_SIZE)
            {
                return 1;
            }
            else
            {
                obstacle->position.y += FIREBALL_SPEED;
            }
            break;
        case 4:
            if (obstacle->position.x < CASE_OFFSET_X - CASE_SIZE)
            {
                return 1;
            }
            else
            {
                obstacle->position.x -= FIREBALL_SPEED;
            }
            break;
        }
    }
    return 0;
}

int distance(POSITION pos1, POSITION pos2)
{
    return (int)sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}

bool detectColision(POSITION pos1, POSITION pos2)
{
    if (distance(pos1, pos2) < SPRITE_SIZE / 2)
    {
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