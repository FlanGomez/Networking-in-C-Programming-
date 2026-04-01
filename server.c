#include <stdio.h>
#include <unistd.h>    // contain all read() write() .. etc
#include <arpa/inet.h> // contain are networking tools

int main(void) {
    int server, client;
    struct sockaddr_in addr; // called sockaddr_in from arpa/inet and created an instance addr
    char buf[1024] = {0};
    socklen_t len = sizeof(addr);  // asked for the length of the instance addr

    server = socket(AF_INET, SOCK_STREAM, 0); // socket needs socket(domain , type , protocol)
                                              // AF_INET : IPv4 address
                                              // SOCK_STREAM : TCP
                                              // Protocal : 0 which is default set by the OS

                                        
    addr.sin_family      = AF_INET;         // Tell OS its a IPv4 address
    addr.sin_port        = htons(8000);     // Assign port where the communiction gonna take place
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Uses the system IP address to communicate

    bind(server, (struct sockaddr *)&addr, sizeof(addr));   // Bind the whole struct by transferring the address through a pointer

    listen(server, 3);                        // Listen()  sees any connection is present and ready for 3 client connection
    printf("Waiting for connection...\n");

    client = accept(server, (struct sockaddr *)&addr, &len);  // accepted the clients struct his data through the pointer

    read(client, buf, sizeof(buf)); // read whatever client sent
    printf("Client: %s\n", buf);

    write(client, "Hello Client How are You", 24); // send our data to client

    close(client); // closes the connection from client
    close(server);
    return 0;
}