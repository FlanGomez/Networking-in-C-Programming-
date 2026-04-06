// #========  ALL INCLUDES =======#
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
// #========  ALL INCLUDES =======#


int client_server;


void *read_messages(void *null){
    char msg[1024];
    while (1)
    {
        read(client_server,msg,sizeof(msg));
        printf("\nClient : %s",msg);
    }
    return NULL;
}


int main(void){
pthread_t thread;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Variables
char msg[1024];

// ---- 16 byte packet struct
struct sockaddr_in addr;
// AF_INET   ==>   2byte (Which IPv connection needed)
// PORT      ==>   2byte
// IP        ==>   4byte
// PADDING   ==>   8byte
// TOTAL           16byte


socklen_t len = sizeof(addr);


client_server = socket(AF_INET,SOCK_STREAM,0);          // IPv4/IPv6     TCP/UDP      PROTOCOL
if(client_server == -1){
    printf("Socket Failed");
}


// SYS Settings to bind with server
addr.sin_family = AF_INET;
addr.sin_port = htons(8000);
inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);

if(connect(client_server,(struct sockaddr *)&addr,sizeof(addr)) == -1){
    printf("Connect Failed");
}


while (1)
{
    memset(msg,0,sizeof(msg));
    printf("\nMessage : ");
    fgets(msg,sizeof(msg),stdin);
    write(client_server,msg,strlen(msg));

    pthread_create(&thread,NULL,read_messages,NULL);
    pthread_detach(thread);
}


}