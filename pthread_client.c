


#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


int main(void){

    int client_server;
    struct sockaddr_in client_address;

    char messages[1024];

    client_server = socket(AF_INET,SOCK_STREAM,0); // created a client_server using socket

    //set client server address
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(5000);
    inet_pton(AF_INET,"127.0.0.1",&client_address.sin_addr); // created a inet_pton and has parameter IPv4 , src , dst

    connect(client_server,(struct sockaddr *)&client_address,sizeof(client_address));
    printf("Connected to Server"); // Created connect() passing the client server and the 16 byte packet sockaddr has passing the sizeof the address

    while (1)
    {

        memset(messages,0,sizeof(messages));
        printf("\nMessage : ");
        fgets(messages,sizeof(messages),stdin);
        write(client_server,messages,strlen(messages));
        
    
        memset(messages,0,sizeof(messages));
        read(client_server,messages,sizeof(messages));
        printf("\nClient : %s\n",messages);

    }
    

    close(client_server);


}