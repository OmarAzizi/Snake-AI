#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Graphics
#include <SDL2/SDL.h>
#include "glyphs.h"

// some macros
#define     WINDOW_WIDTH    1800
#define     WINDOW_HEIGHT   1000
#define     DELAY_TIME      5

// exit codes
#define     EXIT_SUCCESS    0
#define     EXIT_FALIUER    1

// Playground
#define     GRID_SIZE       88
#define     GRID_DIM        850

void render_grid(SDL_Renderer*, int, int);

enum { SNAKE_UP, SNAKE_DOWN, SNAKE_RIGHT, SNAKE_LEFT }; // for snake
enum { TRY_FORWARD, TRY_LEFT, TRY_RIGHT }; // for AI

// Snake stuff
typedef struct Snake {
    int x;
    int y;
    int dir;

    struct Snake* next;
} Snake;

typedef struct Snake snake;
void init_snake();
void render_snake(SDL_Renderer*, int, int);
void move_snake();
void increase_snake();

typedef struct Apple {
    int w;
    int h;
    int x;
    int y;
    int score;
    int top_score;
} Apple;

struct Apple apple;

void gen_apple();
void render_apple(SDL_Renderer*, int, int);
void detect_apple();

void reset_snake();
void detect_crash();

// Functions for the AI agent
void turn_left();
void turn_right();
void ai();

/* 
    The algorithm of the AI agent (Reinforcement Learning)
*/
int state(int);

// score rendering
void render_score(SDL_Renderer*, int, int);
void render_top_score(SDL_Renderer*, int, int);

#endif
