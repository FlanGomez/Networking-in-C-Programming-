

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

int client_grouping[5];
int per_client = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;



void *handle(void *ID){  // This functions doesnt know the TYPE it is kept as void(But we can assign a type like int etc)
    int client = *(int *)ID; // pointer casting used where integer datatype assigned to ID
    free(ID);
    char message[1024];


    while (1)
    {
                if (read(client, message, sizeof(message)) <= 0) {
                    printf("Client disconnected\n");
                    break;
                }
                printf("\nClient: %s\n", message);

                for(int each_ID = 0;each_ID < 5;each_ID++){
                    pthread_mutex_lock(&lock);
                    if(client_grouping[each_ID] != client && client_grouping[each_ID] != 0)
                    {
                        write(client_grouping[each_ID],message,strlen(message));
                    }
                pthread_mutex_unlock(&lock);
                
            }
        }

    close(client);
    return NULL;
    


}

int main(){

int server,client;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
pthread_t thread;

socklen_t len_CLIENT = sizeof(client_address);


server = socket(AF_INET,SOCK_STREAM,0);
if(server == -1)
{
    printf("Socket Failed\n");
    exit(1);
}

server_address.sin_family = AF_INET;
server_address.sin_port = htons(8000);
server_address.sin_addr.s_addr = htonl(INADDR_ANY);

if(bind(server,(struct sockaddr *)&server_address, sizeof(server_address)) == -1){
    printf("Bind Failed\n");
    exit(1);
}


if(listen(server,3)== -1){
    printf("Listen Failed\n");
    exit(1);
}
printf("Listening....\n");

while (1)
{
    client = accept(server,(struct sockaddr *)&client_address, &len_CLIENT); // This return Client ID like 1 , 2 , 3 ....
    if(client == -1)
    {
        printf("Accept Failed");
        continue;
    }
    pthread_mutex_lock(&lock);
    client_grouping[per_client] = client;
    per_client++;
    pthread_mutex_unlock(&lock);

    printf("Client ID : %d\n",client);
    int *Client_ID = malloc(sizeof(int));
    *Client_ID = client;

    pthread_create(&thread,NULL,handle,Client_ID); // 1.Thread ID   2. NULL (for default settings)   3. Function   4.arg
    pthread_detach(thread);


}


}
