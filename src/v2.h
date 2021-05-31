#pragma once
#include <math.h>

#define MIN(x, y)        ((x) < (y) ? (x) : (y))
#define MAX(x, y)        ((x) > (y) ? (x) : (y))
#define ROUND(x)         ((x) < 0.5f ? (int)(x) : (int)(x) + 1)
#define CLAMP(x, lo, hi) (MAX((lo), MIN((x), (hi))))

//---------------------------------------------------------------------------------------------------------------

typedef struct V2
{
    float x, y;
} V2;

//---------------------------------------------------------------------------------------------------------------

static inline V2 v2(float x, float y)                   { V2 v = {x, y}; return v; }

static inline V2 v2_add(V2 a, V2 b)                     { return v2(a.x + b.x, a.y + b.y); }
static inline V2 v2_sub(V2 a, V2 b)                     { return v2(a.x - b.x, a.y - b.y); }
static inline V2 v2_mul(V2 a, V2 b)                     { return v2(a.x * b.x, a.y * b.y); }
static inline V2 v2_div(V2 a, V2 b)                     { return v2(a.x / b.x, a.y / b.y); }

static inline V2 v2_add_s(V2 a, float s)                { return v2(a.x + s, a.y + s); }
static inline V2 v2_sub_s(V2 a, float s)                { return v2(a.x - s, a.y - s); }
static inline V2 v2_mul_s(V2 a, float s)                { return v2(a.x * s, a.y * s); }
static inline V2 v2_div_s(V2 a, float s)                { return v2(a.x / s, a.y / s); }

static inline V2 v2_clamp(V2 a, float lo, float hi)     { return v2(CLAMP(a.x, lo, hi), CLAMP(a.y, lo, hi)); }

static inline V2 v2_rotate(V2 a, float r) 
{
    float s = sinf(r);
    float c = cosf(r); 
    return v2(c * a.x - s * a.y, s * a.x + c * a.y);
}

//---------------------------------------------------------------------------------------------------------------