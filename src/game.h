#pragma once
#include "entity.h"
#include "renderer.h"
#include "window.h"

#define MAX_ASTEROIDS 64
#define MAX_BULLETS 3

#define ASTEROID_VERTS 20

//-------------------------------------------------------------------------------------------

typedef struct Game Game;

//-------------------------------------------------------------------------------------------

void game_init();
void game_shutdown();

void game_run();
void game_render();
void game_reset();

//-------------------------------------------------------------------------------------------

V2 a_model[ASTEROID_VERTS];
V2 b_model[4];
V2 p_model[3];

struct Game
{
    Window* window;
    Renderer* renderer;

    Entity player;
    int player_score;
    
    Entity asteroids[MAX_ASTEROIDS];
    int num_asteroids;

    Entity bullets[MAX_BULLETS];
    int num_bullets;

    int level;
};

//-------------------------------------------------------------------------------------------

Game game;

//-------------------------------------------------------------------------------------------
