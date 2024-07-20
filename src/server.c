// game lib
#include "player.h"
#include "game.h"

// stdlib
#include <stdio.h>
#include <string.h>
#include <errno.h>

// networking stuff
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#define PORT "44556"
#define ROOM_SIZE 2


int main(int argc, char **argv) {
    if (argc != 2) {
        perror("Missing argument!\nUsage: ./server <local ip>");
        return -1;
    }
    char *server_ip = argv[1];

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

        ret = bind(sock, it->ai_addr, it->ai_addrlen);
        if (ret == -1) continue;
    }

    if (sock == -1) {
        perror("Couldn't create socket.\n");
        freeaddrinfo(res);
        return -1;
    }

    ret = listen(sock, ROOM_SIZE);
    if (ret == -1) {
        fprintf(stderr, "ERROR: %d, %s\n Couldn't listen for connections\n", errno, strerror(errno));
        freeaddrinfo(res);
        return -1;
    }

    struct sockaddr sock_p1;
    socklen_t sock_len = sizeof(sock_p1);
    int sock_fd_p1;
    
    sock_fd_p1 = accept(sock, &sock_p1, &sock_len);
    if (sock_fd_p1 == -1) {
        perror("Couldn't accept connection.\n");
        freeaddrinfo(res);
        return -1;
    }


    struct sockaddr_in sock_in_p1 = *(struct sockaddr_in*) &sock_p1;
    char p1_ip[INET_ADDRSTRLEN];
    if (!inet_ntop(sock_in_p1.sin_family, &sock_in_p1.sin_addr, p1_ip, sock_len)) {
        perror("couldn't get ip from socket\n");
        freeaddrinfo(res);
        return -1;
    }

    printf("connected to %s!\n", p1_ip);

    printf("attempting to send player info.\n");

    Player p1;
    init_player(&p1, BLUE);
    p1.pos = (Vector2) {(float)WORLD_W / 2, (float)WORLD_H / 2};
    send(sock_fd_p1, &p1, sizeof(p1), 0);

    printf("sent!\n");

    return 0;
}
