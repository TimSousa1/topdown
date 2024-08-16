#ifndef PACKETS_H
#define PACKETS_H
#include <raylib.h>
#include "game.h"

typedef struct {

    struct {
        Vector2 move_dir;
        Vector2 dir;
    } player;

} packet_input;

typedef struct {

    int connected:1;

    struct {
        int id;
        int alive:1;
        Vector2 pos;
        Vector2 dir;
    } players[ROOM_SIZE];

} packet_output;

#endif
