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


// reads from file descriptor `read_from` and returns error status on `*error`
// b and n will also be written to `bo` and `no` if provided
packet_output read_packet_out(int read_from, int *error, int *bo, int *no);

// packet `p`, number of bytes read `b`, number of times ran recv `n`
void print_packet_out(packet_output p, int b, int n);

#endif
