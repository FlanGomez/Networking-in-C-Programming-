
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void){

    int server_main;
    int client;

    char messages[1024];

    struct sockaddr_in server_address;
    struct sockaddr_in client_address; // keeps it empty for users details to be filled

    socklen_t client_len = sizeof(client_address);


    server_main = socket(AF_INET,SOCK_STREAM,0);

    // --------- Interior Structure of sockaddr_in  for Server_address  -------- //
    // 2 bytes sin_family(TCP or UDP connection)
    // 2 bytes sin_port (Any Port Number)
    // 4 bytes sin_addr (IP Address)
    // 8 byte Padding
    // --------- Interior Structure of sockaddr_in  for Server_address  -------- //

    server_address.sin_family = AF_INET;                 // 2 bytes
    server_address.sin_port = htons(5000);               // 2 bytes
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);  // 4 bytes
    // 8 bytes of Padding

    // Total : 16 bytes Packet


    bind(server_main,(struct sockaddr *)&server_address , sizeof(server_address)); // OS will either deny or accept the packet of 16 bytes you created

    listen(server_main,5);
    printf("\nServer Listening ...........\n");

    while (1)
    {
        //What is fork()
        // When the Server handles more than one clients
        // fork creates parent and child process id where we use child process id to create a replica using the same memory of the Parent process id
        // This way the server talks to multiple client simantenousily but the clients cant talk to each other
        client = accept(server_main,(struct sockaddr *)&client_address , &client_len);

        pid_t child_pid = fork(); // Created a fork() to create a child process id

        if(child_pid == 0){  // if the child_pid is equal to zero then close the main server and ask for write() and read()

            close(server_main);

            memset(messages,0,sizeof(messages));
            printf("\nMessage : ");
            fgets(messages,sizeof(messages),stdin);
            write(client,messages,strlen(messages));

            memset(messages,0,sizeof(messages));
            read(client,messages,sizeof(messages));
            printf("\nClient : %s\n",messages);

            close(client);
            exit(0);

            
        }

        else{
            close(client);
        }
    

    }
    









}

