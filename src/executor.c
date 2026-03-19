#include <fcntl.h>
#include "executor.h"
#include <linux/uinput.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

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
            printf("Moving mouse by X: %d, Y: %d\n", (int16_t)ntohs(pidgeon->x), (int16_t)ntohs(pidgeon->y));
            // possible logic thereof
            break;
        
        default:
            printf("Idk what that is, I'm not doing that\n");
            break;
    }
}

int open_virtual_device() {
    printf("Trying to create new virtual device...\n");

    int id = -1;
    id = open("/dev/uinput", O_WRONLY, O_NONBLOCK); // open a gateway to virtual device hell

    if(id < 0)
        exit(-1);

    ioctl(id, UI_SET_EVBIT, EV_KEY); // defines that it has keys
    ioctl(id, UI_SET_KEYBIT, BTN_LEFT); // defines that it can press left click

    struct uinput_setup setup; // we have to use the uinput_setup provided in linux/uinput.h
    memset(&setup, 0, sizeof(setup));

    strncpy(setup.name, "Remote", UINPUT_MAX_NAME_SIZE); // copy remote name to it
    setup.id.bustype = BUS_USB; // defines that it's an usb
    setup.id.vendor = 0x453; // some random jangle tbh idk
    setup.id.product = 0x5643;

    // defining that it can playpause, volume up and down
    ioctl(id, UI_SET_KEYBIT, KEY_PLAYPAUSE);
    ioctl(id, UI_SET_KEYBIT, KEY_VOLUMEUP);
    ioctl(id, UI_SET_KEYBIT, KEY_VOLUMEDOWN);

    // defining it's also a mouse and can move
    ioctl(id, UI_SET_EVBIT, EV_REL);
    ioctl(id, UI_SET_RELBIT, REL_X);
    ioctl(id, UI_SET_RELBIT, REL_Y);

    //final call to summon the remote
    ioctl(id, UI_DEV_SETUP, &setup);
    ioctl(id, UI_DEV_CREATE);

    return id;
}