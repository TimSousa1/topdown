// game
#include "game.h"
#include "player.h"

// stdlib
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

// networking stuff
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#define TICK_RATE 64

struct con_t {
    int sock_fd;
    struct sockaddr sock;
    char ip[INET_ADDRSTRLEN];
};

struct con_t setup_net(char *server_ip) {
    struct con_t server = {0}; // return value
    server.sock_fd = -1;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int ret;
    ret = getaddrinfo(server_ip, PORT, &hints, &res);

    if (ret) {
        fprintf(stderr, "ERROR %d: %s\n", ret, gai_strerror(ret));
        return server;
    }

    int sock_fd;
    for (struct addrinfo *it = res; it; it = it->ai_next) {
        sock_fd = socket(it->ai_family, it->ai_socktype, it->ai_protocol);
        if (sock_fd == -1)
            continue;

        ret = bind(sock_fd, it->ai_addr, it->ai_addrlen);
        if (ret == -1)
            continue;
    }

    if (sock_fd == -1) {
        perror("Couldn't create socket.\n");
        freeaddrinfo(res);
        return server;
    }

    ret = listen(sock_fd, ROOM_SIZE);
    if (ret == -1) {
        perror("Couldn't listen for connections\n");
        freeaddrinfo(res);
        return server;
    }

    strcpy(server.ip, server_ip);
    server.sock_fd = sock_fd;
    server.sock = (struct sockaddr){0}; // not needed

    return server;
}

struct con_t get_connection(int sock_fd) {
    struct con_t con;
    socklen_t sock_len = sizeof(con.sock);

    con.sock_fd = accept(sock_fd, &con.sock, &sock_len);
    if (con.sock_fd == -1) {
        perror("Couldn't accept connection.\n");
    }

    struct sockaddr_in sock_in_p1 = *(struct sockaddr_in *)&con.sock;

    char con_ip[INET_ADDRSTRLEN];
    if (!inet_ntop(sock_in_p1.sin_family, &sock_in_p1.sin_addr, con_ip,
                sizeof(con_ip))) {
        perror("couldn't get ip from socket\n");
    }

    strcpy(con.ip, con_ip);
    return con;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        perror("Missing argument!\nUsage: ./server <local ip>");
        return -1;
    }
    char *server_ip = argv[1];

    struct con_t server = setup_net(server_ip);
    if (server.sock_fd == -1) {
        perror("Couldn't setup server!\n");
        return -1;
    }

    struct con_t cons[ROOM_SIZE];

    for (int i = 0; i < ROOM_SIZE; i++) {
        cons[i] = get_connection(server.sock_fd);
        printf("connected to %s!\n", cons[i].ip);
    }

    printf("attempting to send player info.\n");

    Player players[ROOM_SIZE];
    Color colors[4] = {BLUE, RED, YELLOW, GREEN};
    for (int i = 0; i < ROOM_SIZE; i++) {
        init_player(&players[i], colors[i]);
        players[i].id = i;
        players[i].pos = (Vector2){(float)(i * 2 + 1) * WORLD_W / (4),
            (float)WORLD_H / 2}; // will depend on each map..
        send(cons[i].sock_fd, &players[i], sizeof(players[i]), 0);
    }

    for (int i = 0; i < ROOM_SIZE; i++) {
        print_player(players[i]);
    }

    for (int i = 0; i < ROOM_SIZE; i++) {
        for (int j = 0; j < ROOM_SIZE; j++) {
            if (i == j)
                continue;
            send(cons[i].sock_fd, &players[j], sizeof(players[j]), 0);
        }
    }

    printf("sent!\n");

    return 0;
}
