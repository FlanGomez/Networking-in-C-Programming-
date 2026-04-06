



#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>



int main(){
    char message[1024];
    struct sockaddr_in client_address;
    socklen_t len = sizeof(client_address);


int client_server = socket(AF_INET,SOCK_STREAM,0);
client_address.sin_family = AF_INET;
client_address.sin_port = htons(8000);
inet_pton(AF_INET,"127.0.0.1",&client_address.sin_addr);

connect(client_server,(struct sockaddr *)&client_address,sizeof(client_address));
printf("Client Connected \n");

while (1)
{
        memset(message,0,sizeof(message));
        printf("\nMessage : ");
        fgets(message,sizeof(message),stdin);
        write(client_server,message,strlen(message));

        memset(message,0,sizeof(message));
        if(read(client_server,message,sizeof(message)) <= 0){
            printf(" \nServer Disconnect ");
            break;
        }
        printf("\nClient : %s\n",message);
}

}