#include "renderer.h"
#include "game.h"

static void renderer_wrap_coords_internal(int* x, int* y)
{
    V2 size = game.window->logical_size;
    if (*x <  0)      *x += size.x;
    if (*y <  0)      *y += size.y;
    if (*x >= size.x) *x -= size.x;
    if (*y >= size.y) *y -= size.y;
}

Renderer* renderer_create(Window* window)
{
    Renderer* self = malloc(sizeof(Renderer));
    assert(self);

    self->handle = SDL_CreateRenderer(window->handle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(self->handle);

    self->screen = SDL_CreateTexture(self->handle, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window->logical_size.x, window->logical_size.y);
    assert(self->screen);

    self->raw_size = window->actual_size.x * window->actual_size.y * sizeof(u32);
    
    self->raw = malloc(self->raw_size);
    assert(self->raw);

    return self;
}

void renderer_destroy(Renderer* self)
{
    SDL_DestroyTexture(self->screen);
    SDL_DestroyRenderer(self->handle);

    self->raw_size = 0;

    free(self->raw);
    self->raw = NULL;

    free(self);
    self = NULL;
}

void renderer_clear(Renderer* self, u32 color)
{
    memset(self->raw, color, self->raw_size);
}

void renderer_draw_point(Renderer* self, int x, int y, u32 color)
{
    renderer_wrap_coords_internal(&x, &y);
    self->raw[(int)(y * game.window->actual_size.x + x)] = color;
}

void renderer_draw_line(Renderer* self, int x0, int y0, int x1, int y1, u32 color)
{
    int dx  = abs(x1 - x0);
    int dy  = abs(y1 - y0);
    int sx  = x0 < x1 ? 1 : -1;
    int sy  = y0 < y1 ? 1 : -1;
    int err = dx > dy ? dx : -dy;

    while (1)
    {
        renderer_draw_point(self, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        float e2 = err * 0.5f;
        if (e2 > -dx)
        {
            err -= dy;
            x0  += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0  += sy;
        }
    }
}

static void circle_segment(Renderer* self, int cx, int cy, int x, int y, u32 color)
{
    renderer_draw_point(self, cx + x, cy + y, color);
    renderer_draw_point(self, cx + x, cy - y, color);
    renderer_draw_point(self, cx - x, cy + y, color);
    renderer_draw_point(self, cx - x, cy - y, color);
}

void renderer_draw_circle(Renderer* self, int cx, int cy, float r, u32 color)
{
    int x = r;
    int y = 0;

    if (r == 0)
    {
        renderer_draw_point(self, cx + x, cy + y, color);
    }
    else
    {
        int p = 1 - r;
        while (x >= y)
        {
            circle_segment(self, cx, cy, x, y, color);

            y ++;
            x -= (p > 0);
            p += (p > 0) ? 2 * y - 2 * x + 1 
                        : 2 * y + 1;
            
            if (x < y) break;

            if (x != y)
                circle_segment(self, cx, cy, y, x, color);
        }
    }
}

void renderer_draw_model(Renderer* self, V2* model, int n, V2 t, float r, float s, u32 color)
{
    V2* new_model = malloc(n * sizeof(V2));
    assert(new_model);

    for (int i = 0; i < n; ++i)
    {
        // Rotate, scale, translate
        new_model[i] = v2_rotate(model[i], r);
        new_model[i] = v2_mul_s(new_model[i], s);
        new_model[i] = v2_add(new_model[i], t);
    }

    for (int i = 0; i < n + 1; ++i)
    {
        int j = i + 1;
        renderer_draw_line(self, new_model[i % n].x, new_model[i % n].y, new_model[j % n].x, new_model[j % n].y, color);
    }

    free(new_model);
    new_model = NULL;
}

void renderer_flush(Renderer* self)
{
    SDL_UpdateTexture(self->screen, NULL, self->raw, game.window->actual_size.x * sizeof(u32));
    SDL_RenderCopy(self->handle, self->screen, NULL, NULL);

    SDL_RenderPresent(self->handle);
}