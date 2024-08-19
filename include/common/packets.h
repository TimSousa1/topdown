#ifndef PACKETS_H
#define PACKETS_H
#include <raylib.h>
#include "game.h"
#include "bullets.h"


typedef struct {

    struct {
        int has_shot:1;
        Vector2 move_dir;
        Vector2 look_dir;
    } player;

} packet_input;


typedef struct {

    struct {
        int id;

        // status
        int alive:1;

        // world
        Vector2 pos;
        Vector2 look_dir;

        bullet bullets[MAX_BULLETS];
    } players[ROOM_SIZE];

} packet_output;

#endif
