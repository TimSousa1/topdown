// game
#include "game.h"
#include "draw.h"
#include "player.h"

// raylib
#include <raylib.h>
#include <raymath.h>

// stdlib
#include <stdio.h>
#include <string.h>

// networking stuff
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>


int setup_net(char *server_ip) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int ret;
    ret = getaddrinfo(server_ip, PORT, &hints, &res);

    if (ret) {
        fprintf(stderr, "ERROR %d: %s\n", ret, gai_strerror(ret));
        return -1;
    }

    int sock;
    for (struct addrinfo *it = res; it; it = it->ai_next) {
        sock = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
        if (sock == -1) continue;

        ret = connect(sock, it->ai_addr, it->ai_addrlen);
        if (ret == -1) continue;
    }

    if (sock == -1) {
        perror("Couldn't create socket.\n");
        freeaddrinfo(res);
        return -1;
    }

    struct sockaddr_in sock_in = *(struct sockaddr_in*) &sock;
    socklen_t sock_len = sizeof(sock_in);

    printf("connected to %s!\n", server_ip);
    return sock;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Missing argument..\nUsage: %s <local_ip>", argv[0]);
        return -1;
    }

    Vector2 screen = {SCREEN_W, SCREEN_H};
    Vector2 world = {WORLD_W, WORLD_H};

    InitWindow(screen.x, screen.y, "online multiplayer");
    SetTargetFPS(FPS);
    HideCursor();

    int sock = setup_net(argv[1]);
    Player players[ROOM_SIZE];
    recv(sock, &players[0], sizeof(players[0]), 0);


    for (int i = 0; i < ROOM_SIZE-1; i++) {
        recv(sock, &players[i+1], sizeof(players[i]), 0);
    }
    
    Player *myself = &players[0];
    for (int i = 0; i < ROOM_SIZE; i++) print_player(players[i]);

    while (!WindowShouldClose()) {
        myself->pointer_pos = convert_spaces(GetMousePosition(), screen, world);
        myself->dir = Vector2Subtract(myself->pos, myself->pointer_pos);
        myself->dir = Vector2Scale(myself->dir, -1);

        myself->speed = Vector2Scale(get_move_dir(), myself->movespeed);
        myself->pos = Vector2Add(myself->pos, Vector2Scale(myself->speed, GetFrameTime()));

        ClearBackground(DARKGRAY);
        //print_player(*myself);
        BeginDrawing();
            {
                for (int i = 0; i < ROOM_SIZE; i++) {
                    draw_player(players[i], world, screen);
                }
                draw_debug(*myself, world, screen);
            }
        EndDrawing();
    }

    return 0;
}
