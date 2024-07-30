#ifndef SERVER_H
#define SERVER_H
#include <raylib.h>

typedef struct {
    Vector2 move_dir;
} packet_input;

typedef struct {
    Vector2 pos;
} packet_output;

#endif
