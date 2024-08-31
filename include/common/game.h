#ifndef GAME_H
#define GAME_H

#include <raylib.h>

#define SCREEN_W 1080
#define SCREEN_H 720

#define WORLD_W 150
#define WORLD_H 100

#define FPS 128

#define printv2(a) printf("(%.2f, %.2f)", a.x, a.y)
#define s2v2(a) (Vector2) {a, a}

#define PORT "44556"
#define ROOM_SIZE 2

#endif
