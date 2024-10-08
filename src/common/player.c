// game
#include "player.h"

// stdlib
#include <stdio.h>
#include <string.h>

// raylib
#include <raylib.h>
#include <raymath.h>

const Color player_colors[] = { BLUE, RED, GREEN, YELLOW,
                                ORANGE, PURPLE, DARKBLUE, DARKGREEN };


Color get_player_color(int i) {
    return player_colors[i];
}

void init_player(Player *player, Color color) {
    memset(player, 0, sizeof(*player));
    player->movespeed = DEFAULT_PLAYER_SPEED;
    player->size = DEFAULT_PLAYER_SIZE;
    player->color = color;
    player->pointer_size = DEFAULT_POINTER_SIZE;
    player->dir = (Vector2) {0, 1};
}


void print_player(Player player) {
    printf("id: %d, pos:(%f, %f)\nspeed:(%f, %f)\nsize:(%f)\npointer_pos:(%f, %f)\npointer_size:(%f)\nmovespeed:(%f)\ndir:(%f,%f)\n",
            player.id, player.pos.x, player.pos.y, player.speed.x, player.speed.y, player.size, player.pointer_pos.x, player.pointer_pos.y,
            player.pointer_size, player.movespeed, player.dir.x, player.dir.y);
}


Vector2 get_move_dir(void) {
    Vector2 move_dir = {0};
    move_dir.y -= IsKeyDown(KEY_W);
    move_dir.x -= IsKeyDown(KEY_A);
    move_dir.y += IsKeyDown(KEY_S);
    move_dir.x += IsKeyDown(KEY_D);

    return Vector2Normalize(move_dir);
}
