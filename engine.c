#include "header.h"

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

PLAYER *setPlayer()
{
    PLAYER *player = malloc(sizeof(PLAYER));
    player->position.x = CASE_OFFSET_X;
    player->position.y = CASE_OFFSET_Y;
    player->skin = 1;
    return player;
}

void movePlayer(PLAYER *player, int direction, int Hole[7][7])
{
    player->position.direction = direction;
    switch (direction)
    {
    case 1:
        if (player->position.y > CASE_OFFSET_Y && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE - 1][(player->position.x - CASE_OFFSET_X) / CASE_SIZE] != 1)
        {
            player->position.y -= CASE_SIZE;
        }
        break;
    case 2:
        if (player->position.x < 6 * CASE_SIZE + CASE_OFFSET_X && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE][(player->position.x - CASE_OFFSET_X) / CASE_SIZE + 1] != 1)
        {
            player->position.x += CASE_SIZE;
        }
        break;
    case 3:
        if (player->position.y < 6 * CASE_SIZE && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE + 1][(player->position.x - CASE_OFFSET_X) / CASE_SIZE] != 1)
        {
            player->position.y += CASE_SIZE;
        }
        break;
    case 4:
        if (player->position.x > CASE_OFFSET_X && Hole[(player->position.y - CASE_OFFSET_Y) / CASE_SIZE][(player->position.x - CASE_OFFSET_X) / CASE_SIZE - 1] != 1)
        {
            player->position.x -= CASE_SIZE;
        }
        break;
    }
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
    switch (direction)
    {
    case 1:
        new->position.y = 6 * CASE_SIZE + CASE_OFFSET_Y;
        new->position.x = rand() % 7 * CASE_SIZE + CASE_OFFSET_X;
        break;
    case 2:
        new->position.x = CASE_OFFSET_X;
        new->position.y = rand() % 7 * CASE_SIZE + CASE_OFFSET_Y;
        break;
    case 3:
        new->position.y = 0 + CASE_OFFSET_Y;
        new->position.x = rand() % 7 * CASE_SIZE + CASE_OFFSET_X;
        break;
    case 4:
        new->position.x = 6 * CASE_SIZE + CASE_OFFSET_X;
        new->position.y = rand() % 7 * CASE_SIZE + CASE_OFFSET_Y;
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

LIST_OBSTACLE updateFireball(LIST_OBSTACLE fireball)
{
    if (fireball.first != NULL)
    {
        OBSTACLE *obstacle = fireball.first;
        while (obstacle != NULL)
        {
            switch (obstacle->position.direction)
            {
            case 1:
                if (obstacle->position.y < CASE_OFFSET_Y-CASE_SIZE)
                {
                    fireball = deleteFromQueue(fireball);
                }
                else
                {
                    obstacle->position.y -= FIREBALL_SPEED;
                }
                break;
            case 2:
                if (obstacle->position.x > CASE_OFFSET_X + 7 * CASE_SIZE)
                {
                    fireball = deleteFromQueue(fireball);
                }
                else
                {
                    obstacle->position.x += FIREBALL_SPEED;
                }
                break;
            case 3:
                if (obstacle->position.y > CASE_OFFSET_Y + 7 * CASE_SIZE)
                {
                    fireball = deleteFromQueue(fireball);
                }
                else
                {
                    obstacle->position.y += FIREBALL_SPEED;
                }
                break;
            case 4:
                if (obstacle->position.x < CASE_OFFSET_X - CASE_SIZE)
                {
                    fireball = deleteFromQueue(fireball);
                }
                else
                {
                    obstacle->position.x -= FIREBALL_SPEED;
                }
                break;
            }

            obstacle = obstacle->next;
        }
    }
    return fireball;
}

bool detectColision(POSITION posplayer, POSITION posobject)
{
    int deltax = posplayer.x - posobject.x;
    int deltay = posplayer.y - posobject.y;
    int distance = sqrt(pow(deltax, 2) + pow(deltay, 2));
    if (distance < SPRITE_SIZE / 2)
    {
        printf("COLLISION\n");
        return true;
    }
    else
    {
        return false;
    }
}

POSITION randomTeleport(POSITION position, int Hole[7][7])
{
    int x = 0;
    int y = 0;
    do
    {
        x = CASE_OFFSET_X + rand() % 7 * CASE_SIZE;
        y = CASE_OFFSET_Y + rand() % 7 * CASE_SIZE;
    } while ((position.x == x && position.y == y) || (Hole[(y- CASE_OFFSET_Y)/CASE_SIZE][(x - CASE_OFFSET_X) / CASE_SIZE]));
    position.x = x;
    position.y = y;
    printf("%d | %d\n",x,y);
    return position;
}
