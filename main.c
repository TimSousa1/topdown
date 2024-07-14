#include <stdio.h>
#include <string.h>
#include <math.h>

#include <raylib.h>
#include <raymath.h>

#define SCREEN_W 1080
#define SCREEN_H 720

#define WORLD_W 150
#define WORLD_H 100

#define FPS 60

#define DEFAULT_PLAYER_SIZE 2.3
#define DEFAULT_POINTER_SIZE 1
#define DEFAULT_PLAYER_SPEED 1

#define s2v2(a) (Vector2) {a, a}


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




void print_player(Player player) {
    printf("pos:(%f, %f)\nspeed:(%f, %f)\nsize:(%f)\npointer_pos:(%f, %f)\npointer_size:(%f)\nmovespeed:(%f)\ndir:(%f,%f)\n",
            player.pos.x, player.pos.y, player.speed.x, player.speed.y, player.size, player.pointer_pos.x, player.pointer_pos.y,
            player.pointer_size, player.movespeed, player.dir.x, player.dir.y);
}


Vector2 convert_spaces(Vector2 vec, Vector2 spacefrom_size, Vector2 spaceto_size){
    Vector2 ret;
    ret.x = vec.x * (spaceto_size.x / spacefrom_size.x);
    ret.y = vec.y * (spaceto_size.y / spacefrom_size.y);
    return ret;
}


void init_player(Player *player, Color color) {
    memset(player, 0, sizeof(*player));
    player->movespeed = DEFAULT_PLAYER_SPEED;
    player->size = DEFAULT_PLAYER_SIZE;
    player->color = color;
    player->pointer_size = DEFAULT_POINTER_SIZE;
}


void draw_player(Player player, Vector2 world, Vector2 screen) {
    Vector2 player_screen_pos, player_screen_size;
    Vector2 pointer_screen_pos, pointer_screen_size;

    pointer_screen_size = convert_spaces(s2v2(player.pointer_size), world, screen);
    player_screen_size = convert_spaces(s2v2(player.size), world, screen);

    pointer_screen_pos = Vector2Add(convert_spaces(player.pointer_pos, world, screen), Vector2Scale(pointer_screen_size, -.5));
    player_screen_pos = convert_spaces(player.pos, world, screen);

    float angle = RAD2DEG * atanf(player.dir.y / player.dir.x);

    DrawRectanglePro((Rectangle) {
                .x = player_screen_pos.x, 
                .y = player_screen_pos.y,
                .width = player_screen_size.x,
                .height = player_screen_size.y
            },
            (Vector2) {player_screen_size.x/2, player_screen_size.y/2}, angle, player.color);

    DrawRectangleV(pointer_screen_pos, pointer_screen_size, BLACK);
}

void draw_debug(Player player, Vector2 world, Vector2 screen) {
    Vector2 player_screen_pos = convert_spaces(player.pos, world, screen);
    Vector2 dir_norm_screen = Vector2Add(convert_spaces(Vector2Scale(Vector2Normalize(player.dir), player.size), world, screen), player_screen_pos);

    DrawLine(player_screen_pos.x, player_screen_pos.y, dir_norm_screen.x, dir_norm_screen.y, GREEN);
}

int main(void) {
    Vector2 screen = {SCREEN_W, SCREEN_H};
    Vector2 world = {WORLD_W, WORLD_H};

    InitWindow(screen.x, screen.y, "online multiplayer");
    SetTargetFPS(FPS);
    HideCursor();

    Player myself;
    init_player(&myself, RED);
    myself.pos = (Vector2) {world.x/2, world.y/2};

    while (!WindowShouldClose()) {
        myself.pointer_pos = convert_spaces(GetMousePosition(), screen, world);
        myself.dir = Vector2Subtract(myself.pos, myself.pointer_pos);
        myself.dir = Vector2Scale(myself.dir, -1);

        ClearBackground(DARKGRAY);
        print_player(myself);
        BeginDrawing();
            {
                draw_player(myself, world, screen);
                draw_debug(myself, world, screen);
            }
        EndDrawing();
    }

    return 0;
}
