#include "packets.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

// reads from file descriptor `read_fd` and returns error status on `*error`
// b and n will also be written to `bo` and `no` if provided
packet_output read_packet_out(int read_fd, int *error, int *bo, int *no) {
    char b_pack[sizeof(packet_output)];
    int b, n, c;

    int to_read = PACK_SIZE;

    for (b = 0, n = 0, c = 0; b < sizeof(b_pack); n++) {
        if (sizeof(b_pack) - b < PACK_SIZE) to_read = sizeof(b_pack) - b;
        c = recv(read_fd, b_pack + b, to_read, 0);
        if (c == -1) {perror("ERROR: read_packet_out");continue;}

        b += c;
    }

    if (c == -1) *error = 1;

    if (bo) *bo = b;
    if (no) *no = n;

    packet_output pack;
    memcpy(&pack, b_pack, sizeof(pack));

    return pack;
}

int send_packet_out(int send_fd, packet_output pack, int *error, int *bo, int *no) {
    int b, n, c;

    int to_send = PACK_SIZE;
    char r_pack[PACK_SIZE];
    char *b_pack = (char *) &pack;
    for (b = 0, n = 0, c = 0; b < sizeof(pack); n++) {
        if (sizeof(pack) - b < PACK_SIZE) to_send = sizeof(pack) - b;
        memcpy(r_pack, b_pack + b, to_send);

        c = send(send_fd, r_pack, to_send, 0);
        if (c == -1) {perror("ERROR: send_packet_out");continue;}

        b += c;
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
