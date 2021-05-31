#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "v2.h"

//----------------------------------------------------------------------------------------------------------------

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define RGBA_BLACK  0x00000000
#define RGBA_WHITE  0xFFFFFFFF
#define RGBA_YELLOW 0xFFFF00FF

#define TAU 6.2831853072f // PI * 2

#define BLOCK_TIMER(name, func) do\
    {\
        clock_t clock_start = clock();\
		func\
		clock_t clock_diff = clock() - clock_start;\
		fprintf(stderr, "[%s: %d] [TIMER] %s took %f ms\n", __FILE__, __LINE__, #name, (double)(clock_diff / (double)CLOCKS_PER_SEC) * 1000.0);\
	} while(0)

#define SWAP(x, y) do\
    {\
        u8 st[sizeof(x) == sizeof(y) ? sizeof(x) : -1]; \
        memcpy(st, &y, sizeof(x)); \
        memcpy(&y, &x, sizeof(x)); \
        memcpy(&x, st, sizeof(x)); \
    } while(0)

#define RANDOM(min, max) ((min) + ((float)rand() / ((float)RAND_MAX / ((max) - (min)))))

//----------------------------------------------------------------------------------------------------------------

typedef unsigned char  u8;
typedef unsigned int   u32;

//----------------------------------------------------------------------------------------------------------------

static inline int point_in_circle(V2 p, V2 c, float r)
{
    float x = p.x - c.x;
    float y = p.y - c.y;
    return (x * x + y * y < r * r);
}

//----------------------------------------------------------------------------------------------------------------