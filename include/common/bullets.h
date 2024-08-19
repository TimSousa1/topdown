#ifndef BULLETS_H
#define BULLETS_H

#include <raylib.h>

#define MAX_BULLETS 64

#define DEFAULT_BULLET_SPEED 120
#define DEFAULT_FIRERATE 5 // bullets per second
#define DEFAULT_BULLET_SIZE 1

#define DEFAULT_BULLET_COLOR PURPLE



typedef struct _bullet {

    // status
    int empty:1;

    // world
    Vector2 pos;
    Vector2 speed;
    int size;
    int movespeed;
    Vector2 move_dir;
    Vector2 look_dir;

    Color color;

} bullet;


typedef struct {
    int bullet_speed;
    int bullet_size;
    Color bullet_color;

    int firerate;
} weapon;

void print_bullet(bullet bullet);
int init_bullet(bullet *b, Vector2 pos, Vector2 look_dir, weapon w);

bullet *find_empty(bullet *a, int num);
int remove_bullet(bullet *b);

int is_out_of_bounds(Vector2 pos, Vector2 world);

#endif
