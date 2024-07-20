#!/bin/sh

gcc client.c player.c draw.c -I ../include -lraylib -lm -o client
./client $1
