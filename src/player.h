#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>

#define DEFAULT_PLAYER_SIZE 2.3
#define DEFAULT_POINTER_SIZE 1
#define DEFAULT_PLAYER_SPEED 1

typedef struct {
    Vector2 pos;
    Vector2 speed;

    float size;
    Vector2 dir;

    Vector2 pointer_pos;
    float pointer_size;

    float movespeed;
    Color color;
} Player;

void init_player(Player *player, Color color);
void print_player(Player player);

#endif
