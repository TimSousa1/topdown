#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "game.h"
#include "bullets.h"

#define DEFAULT_PLAYER_SIZE 2.3
#define DEFAULT_POINTER_SIZE 1
#define DEFAULT_PLAYER_SPEED 30

typedef struct {
    int id;

    Vector2 pos;
    Vector2 speed;

    float size;
    Vector2 look_dir;

    Vector2 pointer_pos;
    float pointer_size;

    float movespeed;
    Color color;

    weapon weapon;
    bullet bullets[MAX_BULLETS];
} Player;


void init_player(Player *player, Color color);
void print_player(Player player);
Vector2 get_move_dir(void);
Color get_player_color(int index);

#endif
