#include "game.h"
#include "draw.h"

#include <raylib.h>
#include <raymath.h>


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

        myself.speed = Vector2Scale(get_move_dir(), myself.movespeed);
        myself.pos = Vector2Add(myself.pos, Vector2Scale(myself.speed, GetFrameTime()));

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
