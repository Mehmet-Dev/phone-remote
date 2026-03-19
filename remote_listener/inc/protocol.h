#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

// Default port and magic number
#define DEFAULT_PORT 6969
#define MAGIC_NUMBER 0xAFAF // as fuck

// define media responses
#define CMD_INVALID 0
#define CMD_PLAY_PAUSE 1
#define CMD_VOLUME_UP 2
#define CMD_VOLUME_DOWN 3
#define CMD_NEXT_TRACK 4
#define CMD_PREV_TRACK 5

// define mouse resposnes
#define CMD_MOUSE_MOVE 10
#define CMD_MOUS_LCLICK 11
#define CMD_MOUSE_RCLICK 12
#define CMD_MOUSE_SCROLL 13

// package struct called envelope (we only use carrier pidgeons)
// packed so that there's no padding which results in more space used in ram
typedef struct {
    uint16_t magic_number;
    uint8_t command;
    int16_t x;
    int16_t y;
} __attribute__((packed)) envelope;

#endif