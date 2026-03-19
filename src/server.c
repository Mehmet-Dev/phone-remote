#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include "arpa/inet.h"
#include <stdio.h>
#include <string.h>

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