#include "window.h"
#include "game.h"

Window* window_create(const char* title, float w, float h, float pw, float ph)
{
    Window* self = malloc(sizeof(Window));
    assert(self);

    self->handle = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    assert(self->handle);

    memset(self->keyboard, 0, sizeof(self->keyboard));

    self->actual_size = v2(w, h);
    self->logical_size = v2(w / pw, h / ph);
    
    self->dt = 0.0;
    self->last_frame = 0.0;
    self->frame_timer = 0.0;
    self->frame_count = 0;
    self->should_close = 0;

    return self;
}

void window_destroy(Window* self)
{
    SDL_DestroyWindow(self->handle);

    memset(self->keyboard, 0, sizeof(self->keyboard));

    free(self);
    self = NULL;
}

void window_update(Window* self)
{
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        self->keyboard[i].pressed = 0;
        self->keyboard[i].released = 0;
    }

    while (SDL_PollEvent(&self->event))
    {
        switch (self->event.type)
        {
            case SDL_QUIT:
                self->should_close = 1;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                window_handle_keyboard(self, self->event.key.keysym.scancode, self->event.type);
                break;
        }
    }

    // Handle frame timing
    const double now = (double)SDL_GetTicks() / 1000.0;
    self->dt = now - self->last_frame;
    self->last_frame = now;
    
    self->frame_timer += self->dt;
    self->frame_count++;
    if (self->frame_timer >= 1.0f)
    {
        self->frame_timer -= 1.0f;
        char title[64];
        snprintf(title, 63, "Asteroids | FPS: %d", self->frame_count);
        window_set_title(self, title);
        self->frame_count = 0;
    }
}

void window_handle_keyboard(Window* self, SDL_Scancode key, u32 action)
{
    switch (action)
    {
        case SDL_KEYDOWN:
            self->keyboard[key].pressed = !self->keyboard[key].down;
            self->keyboard[key].down = 1;
            break;

        case SDL_KEYUP:
            self->keyboard[key].released = 1;
            self->keyboard[key].down = 0;
            break;

        default:
            break;
    }
}

int window_should_close(Window* self)
{
    return self->should_close;
}

void window_set_title(Window* self, const char* title)
{
    SDL_SetWindowTitle(self->handle, title);
}

