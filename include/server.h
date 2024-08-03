#ifndef SERVER_H
#define SERVER_H
#include <raylib.h>
#include "game.h"

typedef struct {

    struct {
        Vector2 move_dir;
    } player;

} packet_input;

typedef struct {

    struct {
        int id;
        Vector2 pos;
    } players[ROOM_SIZE];

} packet_output;

#endif
