#include <fcntl.h>
#include "executor.h"
#include <linux/uinput.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

/// @brief simple method to execute system writes to virtual device
/// @param fd file descriptor id
/// @param type code type used to determine type
/// @param code event code to use
/// @param val 1 = pressing, 0 = letting go, this does not apply to everything as mouse movement uses X and Y coordinates to move up or down
void emit(int fd, int type, int code, int val) {
    struct input_event event;
    memset(&event, 0, sizeof(event)); // clearing out struct so it's not filled with crap

    event.type = type;
    event.code = code;
    event.value = val;
    event.time.tv_sec = 0;

    write(fd, &event, sizeof(event));
}

/// @brief shorthand used for button presses
/// @param fd file descriptor id
/// @param code action code to be executed i.e. KEY_VOLUMEUP
void tap_key(int fd, int code) {
    emit(fd, EV_KEY, code, 1);
    emit(fd, EV_SYN, SYN_REPORT, 0); // write calls to sync is required for it to update
    emit(fd, EV_KEY, code, 0);
    emit(fd, EV_SYN, SYN_REPORT, 0);
}

/// @brief switch statement for handling various commands
/// @param pidgeon envelope package 
/// @param virtual_device_id id of the virtual device (technically file descriptor)
void handle_command(envelope* pidgeon, int virtual_device_id) {
    uint8_t command = pidgeon->command;
    switch(command) {
        case CMD_PLAY_PAUSE: 
            printf("Pausing/playing video...\n");
            tap_key(virtual_device_id, KEY_PLAYPAUSE);
            break;
        case CMD_VOLUME_UP:
            printf("Raising volume...\n");
            tap_key(virtual_device_id, KEY_VOLUMEUP);
            break;
        case CMD_VOLUME_DOWN:
            printf("Lowering volume...\n");
            tap_key(virtual_device_id, KEY_VOLUMEDOWN);
            break;
        case CMD_NEXT_TRACK:
            printf("Next track or video or whatever...\n");
            tap_key(virtual_device_id, KEY_NEXTSONG);
            break;
        case CMD_PREV_TRACK:
            printf("Previous track or video whatever...\n");
            tap_key(virtual_device_id, KEY_PREVIOUSSONG);
            break;
        
        case CMD_MOUSE_MOVE: {
            int16_t x = (int16_t)ntohs(pidgeon->x); // ntohs is required to convert big endian to little endian: 0x1500 -> 0x0015
            int16_t y = (int16_t)ntohs(pidgeon->y);
            printf("Moving mouse by X: %d, Y: %d\n", x, y);
            
            emit(virtual_device_id, EV_REL, REL_X, x);
            emit(virtual_device_id, EV_REL, REL_Y, y);
            emit(virtual_device_id, EV_SYN, SYN_REPORT, 0);

            break;
        }
        case CMD_MOUSE_LCLICK:
            printf("Left clicking on the mouse!\n");
            tap_key(virtual_device_id, BTN_LEFT);
            break;
        case CMD_MOUSE_RCLICK:
            printf("Right clicking on the mouse!\n");
            tap_key(virtual_device_id, BTN_RIGHT);
            break;
        
        default:
            printf("Idk what that is, I'm not doing that\n");
            break;
    }
}

/// @brief creating a virtual device to do certain events
/// @return int of file descriptor/virtual device id
int open_virtual_device() {
    printf("Trying to create new virtual device...\n");

    int id = -1;
    id = open("/dev/uinput", O_WRONLY, O_NONBLOCK); // open a gateway to virtual device hell

    if(id < 0) {
        perror("Virtual device couldn't be created");
        exit(-1);
    }

    ioctl(id, UI_SET_EVBIT, EV_KEY); // defines that it has keys
    ioctl(id, UI_SET_KEYBIT, BTN_LEFT); // defines that it can press left click

    struct uinput_setup setup; // we have to use the uinput_setup provided in linux/uinput.h
    memset(&setup, 0, sizeof(setup));

    strncpy(setup.name, "Remote", UINPUT_MAX_NAME_SIZE); // copy remote name to it
    setup.id.bustype = BUS_USB; // defines that it's an usb
    setup.id.vendor = 0x453; // some random jangle tbh idk
    setup.id.product = 0x5643;

    // defining that it can play, pause, volume up and down
    ioctl(id, UI_SET_KEYBIT, KEY_PLAYPAUSE);
    ioctl(id, UI_SET_KEYBIT, KEY_VOLUMEUP);
    ioctl(id, UI_SET_KEYBIT, KEY_VOLUMEDOWN);

    // defining it's also a mouse and can move
    ioctl(id, UI_SET_EVBIT, EV_REL);
    ioctl(id, UI_SET_RELBIT, REL_X);
    ioctl(id, UI_SET_RELBIT, REL_Y);

    // it should be able to skip to next/previous
    ioctl(id, UI_SET_KEYBIT, KEY_NEXTSONG);
    ioctl(id, UI_SET_KEYBIT, KEY_PREVIOUSSONG);

    //final call to summon the remote
    ioctl(id, UI_DEV_SETUP, &setup);
    ioctl(id, UI_DEV_CREATE);

    return id;
}

/// @brief kill the virtual device and bury the evidence
/// @param fd 
void close_virtual_device(int fd) {
    ioctl(fd, UI_DEV_DESTROY, 0);
}