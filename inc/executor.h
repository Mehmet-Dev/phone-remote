#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "protocol.h"

void handle_command(envelope* pidgeon, int virtual_device_id);
int open_virtual_device();

#endif