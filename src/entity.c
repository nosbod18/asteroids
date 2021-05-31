#include "entity.h"

#include <string.h>
#include <SDL2/SDL.h>
#include "game.h"

static void entity__wrap_coords(float* x, float* y)
{
    V2 size = game.window->logical_size;
    if (*x < 0)       *x += size.x;
    if (*y < 0)       *y += size.y;
    if (*x >= size.x) *x -= size.x;
    if (*y >= size.y) *y -= size.y;
}

Entity entity_create(V2 pos, V2 vel, V2* model, int n, float angle, int size)
{
    Entity e = {
        .pos = pos,
        .vel = vel,
        .angle = angle,
        .size = size,
    };

    memcpy(e.model, model, n * sizeof(V2));
    return e;
}

void entity_update(Entity* e, u8 keep_in_bounds)
{
    e->pos = v2_add(e->pos, v2_mul_s(e->vel, game.window->dt));
    if (keep_in_bounds)
        entity__wrap_coords(&e->pos.x, &e->pos.y);
}


void player_movement(Entity* p, Button* keyboard)
{
    if (keyboard[SDL_SCANCODE_UP].down)
    {
        p->vel.x +=  sinf(p->angle) * 100.0f * game.window->dt;
        p->vel.y += -cosf(p->angle) * 100.0f * game.window->dt;
    }

    if (keyboard[SDL_SCANCODE_LEFT].down)
    {
        p->angle -= 2.5f * game.window->dt;
    }

    if (keyboard[SDL_SCANCODE_RIGHT].down)
    {
        p->angle += 2.5f * game.window->dt;
    }

    if (keyboard[SDL_SCANCODE_SPACE].pressed)
    {
        // Fire a bullet in the direction the player is facing
        if (game.num_bullets < MAX_BULLETS)
        {
            V2 dir = v2(sinf(p->angle), -cosf(p->angle));
            game.bullets[game.num_bullets++] = entity_create(p->pos, v2_mul_s(dir, 100.0f), b_model, 4, 0.0f, 1);
        }
    }

    v2_clamp(p->vel, 0.0f, 1.0f);

    entity_update(p, 1);
}
