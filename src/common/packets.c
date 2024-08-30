#include "packets.h"
#include <stdio.h>
#include <sys/socket.h>

// reads from file descriptor `read_fd` and returns error status on `*error`
// b and n will also be written to `bo` and `no` if provided
packet_output read_packet_out(int read_fd, int *error, int *bo, int *no) {
    packet_output pack;
    int b, n, c;

    for (b = 0, n = 0, c = 0; b < sizeof(pack); n++, b += c) {
        c = recv(read_fd, &pack + b, sizeof(pack) - b, 0);
        if (c == -1) break;
    }

    if (c == -1) *error = 1;

    if (bo) *bo = b;
    if (no) *no = n;

    return pack;
}

int send_packet_out(int send_fd, packet_output pack, int *error, int *bo, int *no) {
    int b, n, c;

    for (b = 0, n = 0, c = 0; b < sizeof(pack); n++, b += c) {
        c = send(send_fd, &pack + b, sizeof(pack) - b, 0);
        if (c == -1) break;
    }

    if (c == -1) *error = 1;

    if (bo) *bo = b;
    if (no) *no = n;

    return *error;
}

// packet `p`, number of bytes read `b`, number of times ran recv `n`
void print_packet_out(packet_output p, int b, int n) {
    printf("---- PACKET ----\n");
    printf("%d/%ld | %d\n", b, sizeof(p), n);
    for (int i = 0; i < ROOM_SIZE; i++) {
        printf("--- id: %d ---\n", p.players[i].id);
        printf("alive: %d\n", p.players[i].id);
        printv2(p.players[i].pos);puts("");
        printv2(p.players[i].look_dir);puts("");
    }
    printf("----------------\n");
}
