#include "executor.h"
#include "server.h"

int main()
{
    envelope carrier_pidgeon;
    struct sockaddr_in client_addr;
    socklen_t addr = sizeof(client_addr);

    int socketId = 0;

    while(socketId == 0) {
        socketId = create_socket();

        if(socketId == 0)
            printf("Trying again...\n");
    }

    printf("Socket created!\n");

    while(1) {
        ssize_t result = recvfrom(socketId, &carrier_pidgeon, sizeof(envelope), 0, (struct sockaddr*)&client_addr, &addr);

        if(result != sizeof(envelope)) {
            perror("Error: result size doesn't match size of envelope, ignoring\n");
            continue;
        }

        if(ntohs(carrier_pidgeon.magic_number) == MAGIC_NUMBER) {
            printf("Hit by magical number! Dude the pidgeon flew away fuck\n");
            handle_command(&carrier_pidgeon);
        }
    }
}