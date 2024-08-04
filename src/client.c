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
        // printf("sent move_dir: "); printv2(pack.move_dir); puts("");
        send(arg.sock_fd, &pack, sizeof(pack), 0); 
    }
    return NULL;
}

void *thread_recv(void *info) {
    thread_arg arg = *(thread_arg*) info;
    packet_output pack;

    int b = 0;
    while (1) {
        b = recv(arg.sock_fd, &pack, sizeof(pack), 0);
        // printf("received move_dir: "); printv2(pack.pos); puts("");
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
    SetWindowMonitor(0);

    // net
    int server_sock = setup_net(argv[1]);
    Player players[ROOM_SIZE];
    Player *myself = NULL;
    packet_output initial;

    for (int i = 0; i < ROOM_SIZE; i++) {
        init_player(&players[i], get_player_color(i));
        players[i].id = i;
    }

    // get all initial player info
    recv(server_sock, &initial, sizeof(initial), 0);

    for (int i = 0; i < ROOM_SIZE; i++) {
        printf("id:%d, pos:(%f, %f)\n", initial.players[i].id, initial.players[i].pos.x, initial.players[i].pos.y);
    }

    for (int i = 0; i < ROOM_SIZE; i++) {
        players[i].pos = initial.players[i].pos;

        if (initial.players[i].id < 0) {
            myself = &players[i];
            myself->id *= -1;
            break;
        }
    }
    for (int i = 0; i < ROOM_SIZE; i++) print_player(players[i]);
    // exit if player is not set
    if (!myself) {
        printf("couldn't set YOUR player!\n");
        return -1;
    }

    for (int i = 0; i < ROOM_SIZE; i++) print_player(players[i]);


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
        Vector2 move_dir;

        myself->pointer_pos = convert_spaces(GetMousePosition(), screen, world);
        myself->dir = Vector2Subtract(myself->pos, myself->pointer_pos);
        myself->dir = Vector2Scale(myself->dir, -1);

        move_dir = get_move_dir();
        myself->speed = Vector2Scale(move_dir, myself->movespeed);
        myself->pos = Vector2Add(myself->pos, Vector2Scale(myself->speed, GetFrameTime()));

        p_send.player.move_dir = move_dir;
        p_send.player.dir = myself->dir;
        b = write(thread_s.pipe_fd[1], &p_send, sizeof(p_send));
        if (b == -1) {
            perror("Couldn't send packet to thread_s!\n");
        }

        while ((b = read(thread_r.pipe_fd[0], &p_recv, sizeof(p_recv))) > 0);

        for (int i = 0; i < ROOM_SIZE; i++) {
            if (i == myself->id) continue; // not caring about what the server thinks of us for now.
            players[i].pos = p_recv.players[i].pos;
            players[i].dir = p_recv.players[i].dir;
        }

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
