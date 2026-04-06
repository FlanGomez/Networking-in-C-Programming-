


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




int client_group[10];
int per_client = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread;

void *client_connection(void *ID){
    int client_ID = *(int *)ID;
    char msg[1024];

    while (1)
    {
        read(client_ID,msg,sizeof(msg));
        printf("\n CLient : %s",msg);

        for(int each_ID = 0;each_ID < 10;each_ID++){
            pthread_mutex_lock(&lock);
            if(client_group[each_ID] != client_ID && client_group[each_ID]!= 0){
                write(client_group[each_ID],msg,strlen(msg));
            }
            pthread_mutex_unlock(&lock);
        }
    }
}






int main(void){

// Variables
int server,client;

// ---- 16 byte packet struct
struct sockaddr_in addr;
struct sockaddr_in client_sys_addr;
// AF_INET   ==>   2byte (Which IPv connection needed)
// PORT      ==>   2byte
// IP        ==>   4byte
// PADDING   ==>   8byte
// TOTAL           16byte


socklen_t len = sizeof(client_sys_addr);


server = socket(AF_INET,SOCK_STREAM,0);          // IPv4/IPv6     TCP/UDP      PROTOCOL
if(server == -1){
    printf("Socket Failed");
}


// SYS Settings to bind with server
addr.sin_family = AF_INET;
addr.sin_port = htons(8000);
addr.sin_addr.s_addr = htonl(INADDR_ANY);

if(bind(server,(struct sockaddr *)&addr,sizeof(addr)) == -1){
    printf("Bind Failed");
}

if(listen(server,3) == -1){
    printf("Listen Failed");
}
printf("Server Listening .........");


while (1)
{
    client = accept(server,(struct sockaddr *)&client_sys_addr,&len);
    if(client == -1){
        printf("Accept Failed");
    }
    pthread_mutex_lock(&lock);
    client_group[per_client] = client;
    per_client++;
    pthread_mutex_unlock(&lock);

    printf("Client ID : %d\n",client);
    int *ID = malloc(sizeof(int));
    *ID = client;

    pthread_create(&thread,NULL,client_connection,ID);
    pthread_detach(thread);
}
}
