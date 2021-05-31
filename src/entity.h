#pragma once
#include "util.h"
#include "v2.h"
#include "window.h"

#define MAX_MODEL_VERTS 20

//----------------------------------------------------------------------------

typedef struct Entity Entity;

//----------------------------------------------------------------------------

Entity  entity_create(V2 pos, V2 vel, V2* model, int n, float angle, int size);
void    entity_update(Entity* e, u8 keep_in_bounds);

void    player_movement(Entity* p, Button* keyboard);

//----------------------------------------------------------------------------

struct Entity
{
    V2 pos;
    V2 vel;
    V2 model[MAX_MODEL_VERTS];
    float angle;
    int size;
};