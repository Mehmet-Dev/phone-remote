#ifndef SERVER_H
#define SERVER_H

#include "protocol.h"

int create_socket();
void start_discovery_thread();
void close_descriptors(int device_id, int socket_id);

#endif