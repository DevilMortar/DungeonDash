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

    if (!IMG_Init(IMG_INIT_PNG))
        printf("\033[1;31mIMG INIT: %s\033[0m\n", IMG_GetError());
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_ExitWithError("SDL | Failed to initialize");
    }
    if (TTF_Init() != 0)
    {
        SDL_ExitWithError("SDL | TTF: Failed to initialize");
    }

    window = SDL_CreateWindow("Dungeon Dash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (window == NULL)
    {
        SDL_ExitWithError("SDL | Failed to create a window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        SDL_ExitWithError("SDL | Failed to create a renderer");
    }

    printf("SDL | Initialized with success !\n");

    /* --------------------------------------- */

    //  Initialisation End et Score
    END *end = malloc(sizeof(END));
    end->texture = newTexture(renderer, "asset/texture/end.png", 400, 200);
    end->skull = newSprite(renderer, "asset/texture/skull.png", 18, 196, SPRITE_SIZE);
    int score;

    // Initialisation Player
    PLAYER *player;
    player = setPlayer();

    // Initialisation Fireball
    LIST_OBSTACLE fireball;
    fireball.sprite = newSprite(renderer, "asset/texture/fireball.png", 3, 74, SPRITE_SIZE);
    fireball.warning = newSprite(renderer, "asset/texture/warning.png", 3, 32, SPRITE_SIZE);
    fireball = setListObstacle(fireball);

    // Initialisation Hole
    int Hole[7][7] = {{0}};

    // Initialisation Coin
    COIN *coin = malloc(sizeof(COIN));
    coin->sprite = newSprite(renderer, "asset/texture/coin.png", 16, 32, SPRITE_SIZE);
    coin->position.direction = 0;

    // Initialisation Global
    score = init(player, &fireball, Hole, coin, renderer);

    // Création de la texture de la grille et des trous
    TEXTURE map = newTexture(renderer, "asset/texture/map1.png", 600, 600);
    TEXTURE hole = newTexture(renderer, "asset/texture/hole.png", 600, 600);


    printf("\nGame statut | Game Initialized !\n");

    /* --------------------------------------- */

    // Set
    SDL_bool program_launched = SDL_TRUE;
    unsigned int loop = 0;
    end->status = 0;

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
                if (end->status == 1)
                {
                    if (event.motion.x > (end->rect2.x - 1) && event.motion.x < (end->rect2.x + end->rect2.w + 1) && event.motion.y > (end->rect2.y - 1) && event.motion.y < (end->rect2.y + end->rect2.h + 1))
                    {
                        end->status = 0;
                        score = init(player, &fireball, Hole, coin, renderer);
                    }
                }
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_z:
                    if (end->status == 0)
                        movePlayer(player, 1, Hole);
                    continue;
                case SDLK_s:
                    if (end->status == 0)
                        movePlayer(player, 3, Hole);
                    continue;
                case SDLK_q:
                    if (end->status == 0)
                        movePlayer(player, 4, Hole);
                    continue;
                case SDLK_d:
                    if (end->status == 0)
                        movePlayer(player, 2, Hole);
                    continue;
                case SDLK_f:
                    printf("Admin | You spawned a fireball\n");
                    fireball = newObstacle(fireball);
                    continue;
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

        // Obstacle
        if (fireball.first != NULL || end->status == 0) {
            if (loop % FIREBALL_MOVELOOP == 0 && loop != 0 && score > 0)
            {
                fireball = updateFireball(fireball);
                if ((double)loop >= 50 * exp((-(float)score) / 30) && end->status == 0)
                {
                    fireball = newObstacle(fireball);
                    loop = 0;
                }
                else
                    loop += 1;
            }
            else
                loop += 1;
        }

        // Collision
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL && end->status == 0)
            {
                if (temp->warning == 0 && detectColision(player->position, temp->position) && end->status == 0)
                {
                    end->status = 1;
                    end->scorerect.x = 6 * WINDOW_WIDTH / 16 + 20;
                    end->scorerect.y = (WINDOW_HEIGHT / 2) - END_SCORE_SIZE / 2;
                }
                temp = temp->next;
            }
        }
        if (detectColision(player->position, coin->position) && end->status == 0)
        {
            coin->position = randomTeleport(coin->position, Hole);
            score += 1;
            printf("Game statut | Score : %d\n", score);
        }

        // Affichage
        displayGame(renderer, player, map, fireball, coin, Hole, hole, loop, end, score);
        SDL_RenderPresent(renderer);
        if (SDL_RenderClear(renderer) != 0)
        {
            SDL_ExitWithError("SDL | Failed to clear renderer");
        }

        // FPS
        frameTime = SDL_GetTicks() - frameStart;
        SDL_LimitFPS(frameTime);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int init(PLAYER *player, LIST_OBSTACLE *fireball, int *Hole, COIN *coin, SDL_Renderer *renderer)
{
    // Initialisation du joueur
    setPlayerSprite(renderer, player);

    // Initialisation des boules de feu
    *fireball = setListObstacle(*fireball);
    *(Hole + (1 * 7) + 1) = 1;
    *(Hole + (5 * 7) + 1) = 1;
    *(Hole + (1 * 7) + 5) = 1;
    *(Hole + (5 * 7) + 5) = 1;
    *(Hole + (3 * 7) + 3) = 1;

    // Initialisation de la pièce
    coin->position = randomTeleport(coin->position, Hole);

    printf("\nGame statut | Game started !\n");
    return 0;
}