// game
#include "game.h"
#include "draw.h"
#include "player.h"
#include "server.h"

// raylib
#include <raylib.h>
#include <raymath.h>

// stdlib
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// networking stuff
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#include <fcntl.h>


typedef struct {
    int pipe_fd[2];
    int sock_fd;
} thread_arg;


void *thread_send(void *info) {
    thread_arg arg = *(thread_arg*) info;
    packet_input pack;

    // sends packet fps times per second
    while (1) {
        read(arg.pipe_fd[0], &pack, sizeof(pack));
        printf("sent move_dir: "); printv2(pack.move_dir); puts("");
        send(arg.sock_fd, &pack, sizeof(pack), 0); 
    }
    return NULL;
}

void *thread_recv(void *info) {
    thread_arg arg = *(thread_arg*) info;
    packet_output pack;

    // receives packet fps times per second
    while (1) {
        recv(arg.sock_fd, &pack, sizeof(pack), 0); 
        printf("received move_dir: "); printv2(pack.pos); puts("");
        write(arg.pipe_fd[1], &pack, sizeof(pack));
    }
    return NULL;
}


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

    // init
    Vector2 screen = {SCREEN_W, SCREEN_H};
    Vector2 world = {WORLD_W, WORLD_H};

    InitWindow(screen.x, screen.y, "online multiplayer");
    SetTargetFPS(FPS);
    HideCursor();

    // net
    int server_sock = setup_net(argv[1]);
    Player players[ROOM_SIZE];
    recv(server_sock, &players[0], sizeof(players[0]), 0);


    for (int i = 0; i < ROOM_SIZE-1; i++) {
        recv(server_sock, &players[i+1], sizeof(players[i]), 0);
    }

    Player *myself = &players[0];
    for (int i = 0; i < ROOM_SIZE; i++) print_player(players[i]);

    Vector2 move_dir;

    // thread
    thread_arg thread_s;
    pipe(thread_s.pipe_fd);
    thread_s.sock_fd = server_sock;

    thread_arg thread_r;
    pipe(thread_r.pipe_fd);
    thread_r.sock_fd = server_sock;
    fcntl(thread_r.pipe_fd[0], F_SETFL, O_NONBLOCK);

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, thread_send, &thread_s);
    pthread_create(&threads[1], NULL, thread_recv, &thread_r);

    packet_input p_send = {0};
    packet_output p_recv = {0};
    int b = 0;

    while (!WindowShouldClose()) {
        myself->pointer_pos = convert_spaces(GetMousePosition(), screen, world);
        myself->dir = Vector2Subtract(myself->pos, myself->pointer_pos);
        myself->dir = Vector2Scale(myself->dir, -1);

        move_dir = get_move_dir();
        myself->speed = Vector2Scale(move_dir, myself->movespeed);
        myself->pos = Vector2Add(myself->pos, Vector2Scale(myself->speed, GetFrameTime()));

        p_send.move_dir = move_dir;
        b = write(thread_s.pipe_fd[1], &p_send, sizeof(p_send));
        if (b == -1) {
            perror("Couldn't send packet to thread_s!\n");
        }

        b = read(thread_r.pipe_fd[0], &p_recv, sizeof(p_recv));
        if (b == -1) {
            perror("Couldn't send packet to thread_s!\n");
        }

        players[1].pos = p_recv.pos;

        ClearBackground(DARKGRAY);
        // print_player(*myself);
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
