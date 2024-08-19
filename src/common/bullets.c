#include "bullets.h"

#include <raylib.h>


int init_bullet(bullet *b, Vector2 pos, Vector2 look_dir, weapon w) {
    if (!b) return -1;

    b->pos = pos;
    b->look_dir = look_dir;
    b->move_dir = look_dir;
    b->speed = (Vector2){0, 0}; // Vector2Scale(b->move_dir, weapon.bullet_speed);
    b->movespeed = w.bullet_speed;
    b->size = w.bullet_size;
    b->color = w.bullet_color;
    b->empty = 0;

    return 0;
}


bullet *find_empty(bullet *a, int num) {
    for (int i = 0; i < num; i++, a++) {
        if (a->empty) return a;
    }

    return 0;
}


int remove_bullet(bullet *b) {
    if (!b) return -1;
    b->empty = 1;
    return 0;
}


int is_out_of_bounds(Vector2 pos, Vector2 world) {
    if (pos.x < 0 | pos.x > world.x) return 1;
    if (pos.y < 0 | pos.y > world.y) return 1;

    return 0;
}
