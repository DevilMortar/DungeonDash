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

    window = SDL_CreateWindow("Dungeon Dash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (window == NULL)
    {
        SDL_ExitWithError("SDL | Failed to create a window");
    }

    if (renderer == NULL)
    {
        SDL_ExitWithError("SDL | Failed to create a renderer");
    }

    printf("SDL | Initialized with sucess !\n");

    /* --------------------------------------- */

    // Initialisation du joueur
    PLAYER *player = setPlayer();
    int score = 0;
    setPlayerSprite(renderer, player);

    // Initialisation des boules de feu
    LIST_OBSTACLE fireball;
    fireball.sprite = newSprite(renderer, "asset/texture/fireball.png", 3, 32, SPRITE_SIZE);
    fireball.warning = newSprite(renderer, "asset/texture/warning.png", 3, 32, SPRITE_SIZE);
    fireball = setListObstacle(fireball);

    // Initialisation des laser
    LIST_OBSTACLE laser;
    laser = setListObstacle(laser);

    // Initialisation des trous
    int Hole[7][7] = {0};
    Hole[2][2] = 1;
    Hole[4][6] = 1;

    // Initialisation de la pièce
    COIN *coin = malloc(sizeof(COIN));
    coin->sprite = newSprite(renderer, "asset/texture/coin.png", 6, 84, SPRITE_SIZE);
    coin->position.direction = 0;
    coin->position.x = 0;
    coin->position.y = 0;
    coin->position = randomTeleport(coin->position, Hole);

    // Création de la texture de la grille et des trous
    TEXTURE map = newTexture(renderer, "asset/texture/map1.png");
    TEXTURE hole = newTexture(renderer, "asset/texture/hole.png");

    printf("\nGame statut | Game started !\n");

    /* --------------------------------------- */

    // Set
    SDL_bool program_launched = SDL_TRUE;
    unsigned int loop = 0;

    // FrameLimiter
    unsigned int frame_limit = SDL_GetTicks() + FPS_DELAY;
    SDL_LimitFPS(frame_limit);
    frame_limit = SDL_GetTicks() + FPS_DELAY;

    // Game Loop
    while (program_launched)
    {
        //FPS check
        frame_limit = SDL_GetTicks() + FPS_DELAY;

        // Contrôle
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                printf("Event listener | Click detected in %d / %d\n",event.motion.x, event.motion.y);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_z:
                    movePlayer(player, 1, Hole);
                    continue;
                case SDLK_s:
                    movePlayer(player, 3, Hole);
                    continue;
                case SDLK_q:
                    movePlayer(player, 4, Hole);
                    continue;
                case SDLK_d:
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
        if (loop % FIREBALL_MOVELOOP == 0 && loop != 0)
        {
            fireball = updateFireball(fireball);
            if (loop == 20)
            {
                fireball = newObstacle(fireball);
                loop = 0;
            }
            else
                loop += 1;
        }
        else
            loop += 1;

        // Collision
        if (fireball.last != NULL)
        {
            OBSTACLE *temp = fireball.first;
            while (temp != NULL)
            {
                if (temp->warning == 0)
                {
                    detectColision(player->position, temp->position);
                }
                temp = temp->next;
            }
        }
        if (detectColision(player->position, coin->position))
        {
            coin->position = randomTeleport(coin->position, Hole);
            score += 1;
            printf("Game statut | Score : %d\n",score);
        }

        // Affichage
        displayGame(renderer, player, map, fireball, laser, coin, Hole, hole, loop);
        SDL_RenderPresent(renderer);
        if (SDL_RenderClear(renderer) != 0)
        {
            SDL_ExitWithError("SDL | Failed to clear renderer");
        }

        // FPS
        //SDL_LimitFPS(frame_limit);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}