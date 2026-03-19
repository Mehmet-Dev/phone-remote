#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "protocol.h"

void handle_command(envelope* pidgeon);
int open_virtual_device();

#endif