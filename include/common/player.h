#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include "game.h"

#define DEFAULT_PLAYER_SIZE 2.3
#define DEFAULT_POINTER_SIZE 1
#define DEFAULT_PLAYER_SPEED 30

#define DEFAULT_BULLET_SPEED 120
#define DEFAULT_FIRERATE 5 // bullets per second
#define DEFAULT_BULLET_SIZE 1

#define DEFAULT_BULLET_COLOR PURPLE


typedef struct _bullet {

    int owner; // who fired the bullet?
    Vector2 pos;
    Vector2 speed;
    int size;
    int movespeed;
    Vector2 move_dir;
    Vector2 look_dir;

    Color color;

    struct _bullet *next;
    struct _bullet *previous;

} bullet;


typedef struct {
    int bullet_speed;
    int bullet_size;
    Color bullet_color;

    int firerate;
} weapon;


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
    bullet *first_bullet;
    bullet *last_bullet;
} Player;


void init_player(Player *player, Color color);
void print_player(Player player);
void print_bullet(bullet bullet);
Vector2 get_move_dir(void);
Color get_player_color(int index);

#endif
