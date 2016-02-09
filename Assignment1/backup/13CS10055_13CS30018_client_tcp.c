#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define SIZE 1000
void error(const char *msg)
{
  perror(msg);
  exit(0);
}
int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;	

  char buffer[SIZE],c,buffer1[SIZE];
  int i = 0;
  if (argc < 3) {
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");
  printf("Choose an Option\n1)Reporter\n2)Reader\n3)exit\n");
  bzero(buffer,SIZE);
  //fgets(buffer,255,stdin);
  scanf("%s",buffer);
  //n = write(sockfd,buffer,strlen(buffer));
  //if (n < 0) 
    //   error("ERROR writing to socket");
  //bzero(buffer,256);
  //n = read(sockfd,buffer,255);
  ///if (n < 0) 
    //   error("ERROR reading from socket");
   //else
  if(strcmp(buffer,"1") == 0)
   {
   		n = write(sockfd,"Reporter",SIZE);
   		if (n < 0) 
              error("ERROR writing to socket");
   		printf("Choose the Section you want to Contribute to-\n");
   		printf("1)Academic \n2)Non-Academic\n");
   		scanf("%s",buffer);
   		if(strcmp(buffer,"1") == 0 || strcmp(buffer,"2") == 0)
   		{
   			if(strcmp(buffer,"1") == 0)
   			n = write(sockfd,"Academic",SIZE);
   			else
   				if(strcmp(buffer,"2") == 0)
   			n = write(sockfd,"Non-Academic",SIZE);
   			if (n < 0) 
              error("ERROR writing to socket");
   			printf("Please Enter the headline \n");
   			//scanf("%s",buffer);
              char ch = getchar(); 
              fgets(buffer,SIZE,stdin);
   			n  = write(sockfd,buffer,SIZE);
   			if (n < 0) 
              error("ERROR writing to socket");
              time_t now = time(NULL);
              strftime(buffer, 20, "%Y-%m-%d", localtime(&now));
   			n = write(sockfd,buffer,SIZE);
   			if (n < 0) 
              error("ERROR writing to socket");
   			printf("Please Enter the Text\n");
   			while ((c = getchar()) != '#') {
      			buffer[i++] = c;
      			}
      			buffer[i] = '\0';
      			i = 0;
   			n = write(sockfd,buffer,SIZE);
   			if (n < 0) 
              error("ERROR writing to socket");
   		}
   		else
   			{
   				printf("Invalid option...\n");
   				close(sockfd);
   			}
   }
   else
   	if(strcmp(buffer,"2") == 0)
   		{
   			write(sockfd,"Reader",SIZE);
   			printf("Choose the Section you want to Read about-\n");
   			printf("1)Academic \n2)Non-Academic\n");
   			scanf("%s",buffer);
   			if(strcmp(buffer,"1") == 0)
   			n = write(sockfd,"Academic",SIZE);
   			else
   				if(strcmp(buffer,"2") == 0)
   			n = write(sockfd,"Non-Academic",SIZE);
   			if (n < 0) 
              error("ERROR writing to socket");
          	printf("Choose what you want to read from list below:\n");
              n = read(sockfd,buffer,SIZE);
              i=0;
              while(buffer[i] != '\0')
              {
                  printf("%c",buffer[i++]);
              }
              char file_number[10];
              scanf("%s",file_number);
              n = write(sockfd,file_number,SIZE);
              if (n < 0) 
              error("ERROR writing to socket");
              bzero(buffer,SIZE);
              n = read(sockfd,buffer,SIZE);
              if (n < 0) 
              error("ERROR reading from socket");
              printf("News in detail\n");
              printf("%s\n\n",buffer);
              // int i=0;
              // while(buffer[i] != '\0')
              // {
              //     printf("%c",buffer[i++]);
              // }
   		}
   		else
   			if(strcmp(buffer,"3") == 0)
   			{
   				printf("Existing\n");
   				close(sockfd);
                  exit(1);
   			}
  //printf("%s\n",buffer);
  close(sockfd);
  return 0;
}
