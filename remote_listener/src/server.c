#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/// @brief creating a socket to send signals to
/// @return the id of the socket
int create_socket() {
    int creation = socket(AF_INET, SOCK_DGRAM, 0); // create a socket

    if(creation == -1) { // in the case it fails for some reason
        perror("Some error occured and the port couldn't be created sorry\n");
        return 0;
    }

    struct sockaddr_in server_addr; // setting up the actual address for the socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // using ipv4
    server_addr.sin_port = htons(DEFAULT_PORT); // using UDP cus it's fast and doesn't require a handshake everytime
    server_addr.sin_addr.s_addr = INADDR_ANY; // default for OS

    int created = bind(creation, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(created < 0) { // if it doesn't bind then yea fuck off
        perror("Couldn't bind socket as the address might be already in use or permission denied\n");
        return 0;
    }

    return creation; // return the id of the descriptor on success
}

void* discovery_beacon(void* arg) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[64];

    addr.sin_family = AF_INET;
    addr.sin_port = htons(6970); // Discovery Port
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    while(1) {
        // This blocks and uses 0% CPU until someone actually shouts
        int len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        if (len > 0 && strncmp(buffer, "WHO_IS_THE_PIGEON_MASTER?", len) == 0) {
            const char* msg = "I_AM_THE_PIGEON_MASTER";
            sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&client_addr, client_len);
            printf("Replied to discovery request from %s\n", inet_ntoa(client_addr.sin_addr));
        }
    }
    return NULL;
}

void start_discovery_thread() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, discovery_beacon, NULL);
    pthread_detach(thread_id); // Let it run independently
}

void close_descriptors(int device_id, int socket_id) {
    close(device_id);
    close(socket_id);
}