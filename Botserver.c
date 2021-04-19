#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#define PORT	 7000
#define MAX_SIZE 1024
#define MAX 7


typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct timeval timeval;


void print_ts(){
time_t ltime;
ltime= time(NULL);
char b[25]="";
for(int i=0;i<24;i++){
b[i]= asctime(localtime(&ltime))[i];
}
printf("%s ",b);
}


int main() {

char  mess[MAX][MAX_SIZE] = {"Hi...", "I am dummy :)","I cannot help you....","Do you know harry potter?","Is it raining outside ?","I don't care ...","Have a good day."};
int i=0;
timeval time_stamp;

int recevfd= socket(AF_INET, SOCK_DGRAM,0);
if(recevfd==-1){
    perror("socket");
    exit(EXIT_FAILURE);
}
sockaddr_in server, client;

server.sin_family= AF_INET;
server.sin_port= htons(PORT);
server.sin_addr.s_addr= INADDR_ANY;



if(bind(recevfd,(sockaddr*)&server, sizeof(sockaddr_in))){
    perror("bind");
    exit(EXIT_FAILURE);
}

printf("[SERVER ON]\n");

char rcv[MAX_SIZE];
while(1){
    int size= sizeof(server);
    int len=recvfrom(recevfd, rcv, sizeof(rcv),0,(sockaddr*)&client,&size);
    gettimeofday(&time_stamp,NULL);
    rcv[len]='\0';
    print_ts(time_stamp);
    printf(" [FROM <%s, %d>]: %s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),rcv);
    //sleep(2);

    sendto(recevfd, &time_stamp, sizeof(time_stamp),0,(sockaddr*)&client, sizeof(client));

    sendto(recevfd, mess[i], strlen(mess[i]),0,(sockaddr*)&client, sizeof(client));
    i=rand()%7;

    }

close(recevfd);
}

