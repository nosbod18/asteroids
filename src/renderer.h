#pragma once
#include <SDL2/SDL.h>
#include "util.h"
#include "v2.h"
#include "window.h"

//-------------------------------------------------------------------------------------------

typedef struct Renderer Renderer;

//-------------------------------------------------------------------------------------------

Renderer*   renderer_create(Window* window);
void        renderer_destroy(Renderer* self);

void        renderer_clear(Renderer* self, u32 color);

void        renderer_draw_point(Renderer* self, int x, int y, u32 color);
void        renderer_draw_line(Renderer* self, int x0, int y0, int x1, int y1, u32 color);
void        renderer_draw_circle(Renderer* self, int x, int y, float r, u32 color);
void        renderer_draw_model(Renderer* self, V2* model, int n, V2 t, float r, float s, u32 color);

void        renderer_flush(Renderer* self);

//-------------------------------------------------------------------------------------------

struct Renderer
{
    SDL_Renderer* handle;
    SDL_Texture* screen;
    u32* raw;
    u32 raw_size;
};

//-------------------------------------------------------------------------------------------