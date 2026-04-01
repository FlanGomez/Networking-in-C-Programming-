#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    int fd;
    struct sockaddr_in addr;
    char buf[1024] = {0};

    // 1. Create socket
    fd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET = IPv4 and SOCK_STREAM = TCP and Protocol is default

    // 2. Set server address
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    // 3. Connect
    connect(fd, (struct sockaddr *)&addr, sizeof(addr)); // Here from the server.c the listen() get connected here to connect() through the data's address
    printf("Connected to server.\n");

    // 4. Write
    write(fd, "Hello from client!", 18);

    // 5. Read
    read(fd, buf, sizeof(buf));
    printf("Server: %s\n", buf);

    // 6. Close
    close(fd);

    return 0;
}