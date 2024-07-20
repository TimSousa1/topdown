#!/bin/sh

gcc server.c player.c -I ../include -lraylib -lm -o server
./server $1
