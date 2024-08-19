#include "bullets.h"

#include <raylib.h>
#include <stdlib.h>


int init_bullet(bullet *b, Vector2 pos, Vector2 look_dir, weapon w) {
    if (!b) return -1;

    b->pos = pos;
    b->look_dir = look_dir;
    b->move_dir = look_dir;
    b->speed = (Vector2){0, 0}; // Vector2Scale(b->move_dir, weapon.bullet_speed);
    b->movespeed = w.bullet_speed;
    b->size = w.bullet_size;
    b->color = w.bullet_color;

    return 0;
}


int add_bullet_to_list(bullet **last, bullet **first) {
    if (!last) return -1;

    bullet *to_add = malloc (sizeof(bullet));
    if (!to_add) return -1;
    
    if (!(*first)) {

        *first = to_add;
        (*first)->next = NULL;
        (*first)->previous = NULL;

        *last = *first;
        return 0;
    }

    to_add->previous = *last;
    (*last)->next = to_add;
    *last = to_add;
    (*last)->next = NULL;

    return 0;
}


int remove_bullet(bullet *b, bullet **first, bullet **last) {
    if (!b | !*first | !*last) return -1;

    if (b == *first) *first = b->next;
    else b->previous->next = b->next;

    if (b == *last) *last = b->previous;
    else b->next->previous = b->previous;

    free(b);

    return 0;
}


int is_out_of_bounds(Vector2 pos, Vector2 world) {
    if (pos.x < 0 | pos.x > world.x) return 1;
    if (pos.y < 0 | pos.y > world.y) return 1;

    return 0;
}
