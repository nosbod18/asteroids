#include "game.h"

#include <SDL2/SDL.h>
#include "util.h"
#include "v2.h"

static int game__on_screen(V2 p)
{
    return (p.x > 0.0f && p.y > 0.0f &&
            p.x < game.window->logical_size.x && p.y < game.window->logical_size.y);
}

void game_init()
{
    assert(SDL_Init(SDL_INIT_VIDEO) == 0);

    srand((u32)time(NULL));

    game.window = window_create("Asteroids | FPS: 0", 640.0f, 480.0f, 4, 4);
    game.renderer = renderer_create(game.window);

    // Create the asteroid model
    for (int i = 0; i < ASTEROID_VERTS; ++i)
    {
        float noise = RANDOM(0.9f, 1.1f);
        a_model[i] = v2(noise * sinf(((float)i / (float)ASTEROID_VERTS) * TAU),
                        noise * cosf(((float)i / (float)ASTEROID_VERTS) * TAU));
    }

    b_model[0] = v2( 1.0f, -1.0f);
    b_model[1] = v2(-1.0f, -1.0f);
    b_model[2] = v2(-1.0f,  1.0f);
    b_model[3] = v2( 1.0f,  1.0f);

    p_model[0] = v2( 0.0f, -5.0f);
    p_model[1] = v2(-2.5f,  2.5f);
    p_model[2] = v2( 2.5f,  2.5f);

    game.player_score = 0;
    game.level = 0;

    game_reset();

    printf("Game successfully initialized!\n");
    printf("Your score: %d\n", game.player_score);
}

void game_shutdown()
{
    memset(game.asteroids, 0, sizeof(game.asteroids));
    memset(game.bullets, 0, sizeof(game.bullets));

    renderer_destroy(game.renderer);
    window_destroy(game.window);
    SDL_Quit();

    printf("Game successfully shutdown!\n");
}

void game_run()
{
    Window* window = game.window;
    Entity* p = &game.player;

    while (!window_should_close(window))
    {
        window_update(window);

        // Update player
        player_movement(p, window->keyboard);

        // Update bullets
        for (int i = 0; i < game.num_bullets; ++i)
        {
            entity_update(&game.bullets[i], 0);

            if (!game__on_screen(game.bullets[i].pos))
                game.bullets[i] = game.bullets[--game.num_bullets];
        }

        // Update asteroids and handle collisions
        for (int i = 0; i < game.num_asteroids; ++i)
        {
            Entity* a = &game.asteroids[i];
            entity_update(&game.asteroids[i], 1);

            // Player collision
            if (point_in_circle(p->pos, a->pos, a->size) && game__on_screen(a->pos))
            {
                printf("Game over! Restarting...\n");
                game.player_score = 0;
                game_reset();
                break;
            }

            for (int j = 0; j < game.num_bullets; ++j)
            {
                Entity* b = &game.bullets[j];

                // Bullet collision
                if (point_in_circle(b->pos, a->pos, a->size) && game__on_screen(a->pos) && game__on_screen(b->pos))
                {
                    if (a->size > 3)
                    {
                        float angle1 = RANDOM(0.0f, TAU);
                        float angle2 = angle1 + M_PI;

                        game.asteroids[game.num_asteroids++] = entity_create(a->pos, v2(sin(angle1) * 10.0f, -cos(angle1) * 10.0f), a_model, ASTEROID_VERTS, angle1, a->size >> 1);
                        game.asteroids[game.num_asteroids++] = entity_create(a->pos, v2(sin(angle2) * 10.0f, -cos(angle2) * 10.0f), a_model, ASTEROID_VERTS, angle2, a->size >> 1);
                    }

                    /* Overwrites the colliding asteroid and bullet with the respective entities at the end of
                     * their arrays and makes the original end unreachable*/
                    game.bullets[j] = game.bullets[--game.num_bullets];
                    game.asteroids[i] = game.asteroids[--game.num_asteroids];

                    switch (a->size)
                    {
                        case 12: game.player_score += 50;  break;
                        case 6:  game.player_score += 100; break;
                        case 3:  game.player_score += 250; break;
                        default:                           break;
                    }

                    printf("Your score: %d\n", game.player_score);
                }
            }
        }

        // The player destroyed all asteroids
        if (game.num_asteroids == 0)
        {
            printf("Level %d cleared!\n", ++game.level);
            game.player_score += 1000;
            game_reset();
        }

        game_render();
    }
}

void game_render()
{
    Renderer* renderer = game.renderer;

    renderer_clear(renderer, RGBA_BLACK);

    for (int i = 0; i < game.num_asteroids; ++i)
    {
        Entity* a = &game.asteroids[i];
        renderer_draw_model(renderer, a->model, ASTEROID_VERTS, a->pos, a->angle, a->size, RGBA_WHITE);
    }

    for (int i = 0; i < game.num_bullets; ++i)
    {
        Entity* b = &game.bullets[i];
        renderer_draw_model(renderer, b->model, 4, b->pos, 0.0f, 0.5f, RGBA_WHITE);
    }

    Entity* p = &game.player;
    renderer_draw_model(renderer, p->model, 3, p->pos, p->angle, 1, RGBA_WHITE);

    renderer_flush(renderer);
}

void game_reset()
{
    game.player = entity_create(v2(game.window->logical_size.x * 0.5f, game.window->logical_size.y * 0.5f), v2(0.0f, 0.0f), p_model, 3, 0.0f, 1.0f);
    game.player.vel = v2(0.0f, 0.0f);

    game.num_asteroids = 0;

    // Start with three asteroids then add one each round
    for (int i = 0; i < game.level + 3; ++i)
    {
        V2 pos = v2(RANDOM(0.0f, game.window->logical_size.x), RANDOM(0.0f, game.window->logical_size.y));

        // Choose a different position for the asteroid if it is within a certain radius from the player
        while (point_in_circle(pos, game.player.pos, 50.0f))
        {
            pos = v2(RANDOM(0.0f, game.window->logical_size.x), RANDOM(0.0f, game.window->logical_size.y));
        }

        game.asteroids[i] = entity_create(pos, 
                                          v2(RANDOM(-5.0f, 5.0f), 
                                             RANDOM(-5.0f, 5.0f)), 
                                          a_model, 
                                          ASTEROID_VERTS, 
                                          RANDOM(0.0f, TAU), 
                                          RANDOM(0.0f, 1.0f) > 0.25f ? 12 : 6);
        game.num_asteroids++;
    }

    memset(game.bullets, 0, sizeof(game.bullets));
    game.num_bullets = 0;
}