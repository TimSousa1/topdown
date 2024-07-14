#ifndef DRAW_H
#define DRAW_H

#include "player.h"
#include <raylib.h>

Vector2 convert_spaces(Vector2 vec, Vector2 spacefrom_size, Vector2 spaceto_size); // convert between two coordinate systems
void draw_player(Player player, Vector2 world, Vector2 screen);
void draw_debug(Player player, Vector2 world, Vector2 screen); // draw debug info like player dir
 
#endif
