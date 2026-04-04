
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>

void *handle_clients(void *arg) {
        // Creates a function pointer keeping datatype as void to pass this function in the pthread_create()
        // The function have parameter which is a pointer void *arg so arg is whatever you pass the data in will be arg replicating it for the function
        // Question What is void *arg
        // void * is an unknown type assgined to the arg like it could be INT , STRING , STRUCT anything and arg is here the memory heap space allocated for each Socket File Descriptor
    
    char messages[1024];
    int client = *(int *)arg; // Created a variable client assigning that the *(int *)arg which means client the data is an integer cause before in the parameter the datatype was void * arg now its an INTEGER
    free(arg);  // Free the arg value after completion

    while (1) {
        memset(messages, 0, sizeof(messages));
        if (read(client, messages, sizeof(messages)) <= 0) {
            printf("Client disconnected\n");
            break;  // exit loop cleanly
        }
        printf("Client : %s\n", messages);

        memset(messages, 0, sizeof(messages));
        printf("Message : ");
        fgets(messages, sizeof(messages), stdin);
        write(client, messages, strlen(messages));
    }

    close(client);  // now reachable ✅
    return NULL;  // This void Function demands a return a value so for this program we dont need to Return so we return NULL
}

int main(void){

    int server_main;
    int client;

    pthread_t thread;  // pthread_t is a type unsigned long


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
        client =  accept(server_main,(struct sockaddr *)&client_address,&client_len);
        // IN every program accept() provides an Integer value which is a SOCKET FILE DESCRIPTOR(SFD) where is return integer value for each client which is 
        //Client_1 = 1  // these integer values are given by accepting each client kind of Integer IDs
        //Client_2 = 2
        int *ptr_client = malloc(sizeof(int));     // Allocted space memory of 4 byte for 32-bit and 64-bit systems for each SFD
        *ptr_client = client; // This means ptr_client is an address 0x5A64 where place the client integer into the address 0x%A64

        pthread_create(&thread,NULL,handle_clients,ptr_client);
        // Blue Print
        // pthread_create(&thread , NULL , function , arg);
        // &thread got from the type pthread_t and OS gives each thread ID to each thread
        // NULL actives the default things
        // function need to perform multi-threading
        // arg the argument passed into the function to handle void *arg


        // why need args
        // args needed to let the function work if the function doesnt need arg can pass arg as NULL
        pthread_detach(thread);
    }
    









}

