#include "executor.h"

void handle_command(envelope* pidgeon) {
    uint8_t command = pidgeon->command;
    switch(command) {
        case CMD_PLAY_PAUSE: 
            printf("Pausing/playing video...\n");
            // possible logic thereof
            break;
        case CMD_VOLUME_UP:
            printf("Raising volume...\n");
            // possible logic thereof
            break;
        case CMD_VOLUME_DOWN:
            printf("Lowering volume...\n");
            // possible logic thereof
            break;
        case CMD_NEXT_TRACK:
            printf("Next track or video or whatever...\n");
            // possible logic thereof
            break;
        case CMD_PREV_TRACK:
            printf("Previous track or video whatever...\n");
            // possible logic thereof
            break;
        
        case CMD_MOUSE_MOVE:
            printf("Moving mouse by X: %d, Y: %d\n", ntohs(pidgeon->x), ntohs(pidgeon->y));
            // possible logic thereof
            break;
        
        default:
            printf("Idk what that is, I'm not doing that\n");
            break;
    }
}