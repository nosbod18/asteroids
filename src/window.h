#pragma once
#include <SDL2/SDL.h>
#include "util.h"
#include "v2.h"

//-------------------------------------------------------------------------------------------

typedef struct Window Window;
typedef struct Button Button;

//-------------------------------------------------------------------------------------------

Window* window_create(const char* title, float w, float h, float pw, float ph);
void    window_destroy(Window* self);
void    window_update(Window* self);
void    window_tick(Window* self);
void    window_handle_keyboard(Window* self, SDL_Scancode key, u32 action);

int     window_should_close(Window* self);
void    window_set_title(Window* self, const char* title);

//-------------------------------------------------------------------------------------------

struct Button
{
    u8 pressed  : 1;
    u8 released : 1;
    u8 down     : 1;
};

struct Window
{
    SDL_Window* handle;
    SDL_Event event;

    Button keyboard[SDL_NUM_SCANCODES];

    V2 actual_size;
    V2 logical_size;

    u32 frame_count;
    double dt;
    double last_frame;
    double frame_timer;

    // Flags
    u8 should_close;
};

//-------------------------------------------------------------------------------------------