
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

#define MAX 100
#define PORT 7000

void print_ts();

int main(int argc, char* argv[]){   //Give IP of server as argument...

struct timeval t_s, start,stop; //time stamp structs

int sendfd= socket(AF_INET, SOCK_DGRAM, 0);
if(sendfd==-1){
    perror("socket");
    exit(EXIT_FAILURE);
    }
struct sockaddr_in server, client;
server.sin_family= AF_INET;
server.sin_port= htons(PORT);
server.sin_addr.s_addr= inet_addr(argv[1]);
char snd[MAX],rcv[MAX];

while(1){
    
    printf("[ENTER MESSAGE]: ");
    scanf("%[^\n]%*c", snd);
    printf("[SENT AT]: ");
    print_ts();
    printf("\n");
    int size= sizeof(client);
    gettimeofday(&start,NULL);  //before sending we are recording time for delay calculation
    sendto(sendfd,snd,strlen(snd),0,(struct sockaddr*)&server,sizeof(server));
    recvfrom(sendfd, &t_s, sizeof(t_s),0,(struct sockaddr*)&client,&size);
    gettimeofday(&stop,NULL);   //After recieving we are recording time  for delay calculation
    int len= recvfrom(sendfd, rcv, sizeof(rcv),0,(struct sockaddr*)&client,&size);
    printf("[SERVER]   ");
    print_ts(t_s);  //Since we need to calculate timestamp at server side so we are storing it in s_t and then printing

    rcv[len]='\0';
    printf(" :  %s",rcv);
    printf("\t\tDELAY: %lu ms\n",(stop.tv_usec - start.tv_usec));     //for nano to mini conversion i am dividing it by 1000
    }
close(sendfd);
}




void print_ts(){
time_t ltime;
ltime= time(NULL);
char b[25]="";
for(int i=0;i<24;i++){
b[i]= asctime(localtime(&ltime))[i];
}
printf("%s ",b);
}



