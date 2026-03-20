#include "executor.h"
#include "server.h"
#include "protocol.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

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

    printf("Socket created on port %d!\n", DEFAULT_PORT);

    start_discovery_thread(); 
    printf("Handshake responder active on port 6970. Waiting for phone to shout...\n");

    int device_id = open_virtual_device(); // virtual device id that's required

    printf("Created virtual device with id: %d\n", device_id);

    while(1) {
        ssize_t result = recvfrom(socketId, &carrier_pidgeon, sizeof(envelope), 0, (struct sockaddr*)&client_addr, &addr);

        if(result != sizeof(envelope)) { // if the size doesn't equal to that of the envelope we dont execute it
            perror("Error: result size doesn't match size of envelope, ignoring\n");
            continue;
        }

        

        if(ntohs(carrier_pidgeon.magic_number) == MAGIC_NUMBER) { // if the magic number matches then we execute
            if (carrier_pidgeon.command == 99) {
                printf("Received Exit Signal. Shutting down gracefully... Peace out!\n");
                break; // This breaks the while(1) loop and exists app
            }
            handle_command(&carrier_pidgeon, device_id);
        }
    }

    close_virtual_device(device_id);
    close_descriptors(device_id, socketId);
    return 0;
}